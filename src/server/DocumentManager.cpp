/*
 * Author: Franco Ruggeri
 */

#include <cte/server/DocumentManager.h>
#include <cte/crdt/SharedEditor.h>
#include <cte/database/DatabaseGuard.h>
#include <cte/database/db_utility.h>
#include <db_utility_secret.h>
#include <QtCore/QVariant>
#include <QtCore/QSet>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace cte {
    DocumentManager::DocumentManager() : mutex_(QMutex::Recursive) {}

    std::optional<DocumentData> DocumentManager::create_document(int session_id, const Document& document) {
        // check inputs
        if (document.name().isEmpty()) throw std::logic_error("document creation failed: invalid document name");

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the document already exists
        query = query_select_document(database, document, true);
        execute_query(query);

        // create document
        std::optional<DocumentData> document_data;
        if (query.size() == 0) {
            QString owner = document.owner();
            QUrl sharing_link = Document::generate_sharing_link(document);

            // insert new document
            query = query_insert_document(database, document, sharing_link);
            execute_query(query);
            query = query_insert_sharing(database, document, owner);
            execute_query(query);

            // open document
            OpenDocument od;
            int site_id = od.open(owner);
            QMutexLocker ml(&mutex_);
            open_documents_.insert(document, od);
            site_ids_[session_id].insert(document, document_data->site_id());
            document_data = DocumentData(site_id, sharing_link);
        }

        // commit transaction
        database.commit();

        return document_data;
    }

    std::optional<DocumentData> DocumentManager::open_document(int session_id, const Document& document,
                                                               const QString& username) {
        // check inputs
        if (opened(session_id, document)) throw std::logic_error("document already opened");

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the document is accessible by the user (selecting also sharing link)
        query = query_select_document(database, document, username);
        execute_query(query);

        // open document
        std::optional<DocumentData> document_data;
        if (query.next()) {
            // load sharing link
            QString sharing_link = query.value("sharing_link").toString();

            // load profiles
            query = query_select_document_profiles(database, document);
            QHash<QString,Profile> profiles;
            while (query.next()) {
                QString username = query.value("username").toString();
                Profile profile(username, query.value("name").toString(),
                                     query.value("surname").toString(),
                                     query.value("icon").value<QImage>());
                profiles.insert(username, profile);
            }

            // open document
            QMutexLocker ml(&mutex_);
            if (!open_documents_.contains(document)) {
                // not in main memory... load text
                query = query_select_document_text(database, document);
                execute_query(query);
                QVector<OpenDocument::character_t> text;
                while (query.next()) {
                    text.push_back({
                                           query.value("index").value<qint32>(),
                                           query.value("value").toString().at(0),
                                           query.value("author").toString()
                                   });
                }
                open_documents_.insert(document, OpenDocument(text));
            }
            OpenDocument& od = open_documents_[document];
            int site_id = od.open(username);
            site_ids_[session_id].insert(document, document_data->site_id());
            document_data = DocumentData(od.text(), site_id, od.cursors(), od.site_ids(), profiles, sharing_link);
        }

        // commit transaction
        database.commit();

        return document_data;
    }

    std::optional<DocumentData> DocumentManager::open_document(int session_id, const QUrl& sharing_link,
                                                               const QString& username) {
        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // load document
        query = query_select_document(database, sharing_link);
        execute_query(query);
        if (!query.next()) throw std::logic_error("invalid sharing link");
        Document document(query.value("owner").toString(), query.value("name").toString());

        // add sharing if not present
        query = query_select_document(database, document, username);
        execute_query(query);
        if (query.size() == 0) {
            query = query_insert_sharing(database, document, username);
            execute_query(query);
        }

        // commit transaction
        database.commit();

        // open document
        return open_document(session_id, document, username);
    }

    void DocumentManager::close_document(int session_id, const Document& document) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id].take(document);
        get_open_document(document).close(site_id);
    }

    void DocumentManager::close_documents(int session_id) {
        QMutexLocker ml(&mutex_);
        auto it_sessions = site_ids_.find(session_id);
        if (it_sessions == site_ids_.end()) return;  // no documents opened
        QHash<Document,int> session_site_ids = *it_sessions;
        for (auto it=session_site_ids.begin(); it != session_site_ids.end(); it++)
            open_documents_[it.key()].close(it.value());
        site_ids_.remove(session_id);
    }

    OpenDocument& DocumentManager::get_open_document(const Document& document) {
        auto it = open_documents_.find(document);
        if (it == open_documents_.end()) throw std::logic_error("document not opened");
        return *it;
    }

    bool DocumentManager::opened(int session_id, const Document& document) const {
        QMutexLocker ml(&mutex_);
        auto it = site_ids_.find(session_id);
        return it != site_ids_.end() && it->contains(document);
    }

    bool DocumentManager::site_id_spoofing(int session_id, const Document& document, const Symbol& symbol) const {
        QMutexLocker ml(&mutex_);
        return opened(session_id, document) && site_ids_[session_id][document] == symbol.site_id();
    }

    void DocumentManager::insert_symbol(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        if (site_id_spoofing(session_id, document, symbol)) throw std::logic_error("site_id spoofing");
        get_open_document(document).insert_symbol(symbol);
    }

    void DocumentManager::erase_symbol(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        get_open_document(document).erase_symbol(symbol);
    }

    void DocumentManager::move_cursor(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        get_open_document(document).move_cursor(site_ids_[session_id][document], symbol);
    }

    QSet<Document> DocumentManager::documents(int session_id, const QString& username) const {
        // open connection
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        QSqlQuery query(database);

        // load accessible documents
        query = query_select_documents(database, username);
        QSet<Document> documents;
        while (query.next()) {
            Document document(query.value("document_owner").toString(), query.value("document_name").toString());
            documents.insert(document);
        }

        // remove already opened ones
        QMutexLocker ml(&mutex_);
        auto it = site_ids_.find(session_id);
        if (it != site_ids_.end())
            for (const auto& document : it->keys())
                documents.remove(document);

        return documents;
    }

    void DocumentManager::save() {
        // copy (so that we do not lock for the entire (slow) saving on the DB) and remove closed documents
        QMutexLocker ml(&mutex_);
        QHash<Document,OpenDocument> open_documents_copy = open_documents_;
        std::remove_if(open_documents_.begin(), open_documents_.end(),
                       [](const OpenDocument& od) { return od.reference_count() == 0; });
        ml.unlock();

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // save documents
        for (auto it=open_documents_copy.begin(); it!=open_documents_copy.end(); it++) {
            const Document& document = it.key();
            QList<Symbol> text = it->text();
            QHash<int,QString> site_ids = it->site_ids();
            QString document_owner = document.owner();
            QString document_name = document.name();

            // delete old document text
            query = query_delete_document_text(database, document);
            execute_query(query);

            // insert updated document text
            query = prepare_query_insert_character(database, document);
            for (unsigned int i=0; i<text.size(); i++) {
                bind_query_insert_character(query, i, site_ids[text[i].site_id()], text[i].value());
                execute_query(query);
            }
        }
    }
}
