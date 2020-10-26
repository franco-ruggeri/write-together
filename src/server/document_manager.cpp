#include <cte/server/document_manager.h>
#include <cte/crdt/shared_editor.h>
#include <cte/database/database_guard.h>
#include <cte/database/database_utility.h>
#include <database_utility_secret.h>
#include <QtCore/QSet>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace cte {
    DocumentManager::DocumentManager() : mutex_(QMutex::Recursive) {}

    std::optional<DocumentInfo> DocumentManager::create_document(int session_id, const Document& document) {
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
        QString owner = document.owner();
        if (query.size() == 0) {
            QUrl sharing_link = Document::generate_sharing_link(document);

            // insert new document
            query = query_insert_document(database, document, sharing_link);
            execute_query(query);
            query = query_insert_sharing(database, document, owner);
            execute_query(query);
        }

        // commit transaction
        database.commit();

        return open_document(session_id, document, owner);
    }

    std::optional<DocumentInfo> DocumentManager::open_document(int session_id, const Document& document,
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
        std::optional<DocumentInfo> document_info;
        if (query.next()) {
            // load sharing link
            QString sharing_link = query.value("sharing_link").toString();

            // load profiles
            query = query_select_document_profiles(database, document);
            execute_query(query);
            QHash<QString,Profile> profiles;
            while (query.next()) {
                Profile profile(query.value("username").toString(),
                                query.value("name").toString(),
                                query.value("surname").toString(),
                                query.value("email").toString(),
                                query.value("icon").toByteArray());
                profiles.insert(profile.username(), profile);
            }

            // open document
            QMutexLocker ml(&mutex_);
            if (!open_documents_.contains(document)) {
                // not in main memory... load text
                query = query_select_document_text(database, document);
                execute_query(query);
                QVector<OpenDocument::character_t> text;
                while (query.next()) {
                    text.push_back({ query.value("index").value<qint32>(), query.value("value").toString().at(0),
                                     query.value("author").toString() });
                }
                open_documents_.insert(document, OpenDocument(text, profiles.keys()));
            }
            OpenDocument& od = open_documents_[document];
            int site_id = od.open(username);
            site_ids_[session_id].insert(document, site_id);

            // get document info
            QHash<QString,std::pair<Profile,QList<int>>> users;
            QHash<int,QString> usernames = od.usernames();
            for (const auto& p : profiles)
                users.insert(p.username(), {p, QList<int>()} );
            for (auto it=usernames.begin(); it != usernames.end(); it++)
                users[it.value()].second.append(it.key());
            document_info = DocumentInfo(od.text(), site_id, od.cursors(), users, sharing_link);
        }

        // commit transaction
        database.commit();

        return document_info;
    }

    std::optional<std::pair<Document,DocumentInfo>>
    DocumentManager::open_document(int session_id, const QUrl& sharing_link, const QString& username) {
        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // load document
        std::optional<std::pair<Document,DocumentInfo>> result;
        query = query_select_document(database, sharing_link);
        execute_query(query);
        if (query.next()) {
            Document document(query.value("owner").toString(), query.value("name").toString());

            // add sharing if not present
            query = query_select_document(database, document, username);
            execute_query(query);
            if (query.size() == 0) {
                query = query_insert_sharing(database, document, username);
                execute_query(query);
            }

            // open document
            DocumentInfo document_info = *open_document(session_id, document, username);
            result = std::make_pair(document, document_info);
        }

        // commit transaction
        database.commit();

        return result;
    }

    int DocumentManager::close_document(int session_id, const Document& document) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id].take(document);
        get_open_document(document).close(site_id);
        return site_id;
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
        return opened(session_id, document) && site_ids_[session_id][document] != symbol.site_id();
    }

    void DocumentManager::insert_symbol(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        if (site_id_spoofing(session_id, document, symbol)) throw std::logic_error("site_id spoofing");
        get_open_document(document).insert_symbol(symbol);
    }

    int DocumentManager::erase_symbol(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        get_open_document(document).erase_symbol(symbol);
        return site_ids_[session_id][document];
    }

    int DocumentManager::move_cursor(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id][document];
        get_open_document(document).move_cursor(site_id, symbol);
        return site_id;
    }

    QList<Document> DocumentManager::get_document_list(const QString& username) const {
        // open connection
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        QSqlQuery query(database);

        // load accessible documents
        query = query_select_documents(database, username);
        execute_query(query);
        QList<Document> documents;
        while (query.next()) {
            Document document(query.value("document_owner").toString(), query.value("document_name").toString());
            documents.append(document);
        }
        return documents;
    }

    QList<Document> DocumentManager::get_open_documents(int session_id) const {
        QMutexLocker ml(&mutex_);
        auto it = site_ids_.find(session_id);
        if (it == site_ids_.end()) return QList<Document>{};
        return it->keys();
    }

    void DocumentManager::save() {
        // copy (so that we do not lock for the entire (slow) saving on the DB) and remove closed documents
        QMutexLocker ml(&mutex_);
        QHash<Document,OpenDocument> open_documents_copy = open_documents_;
        for (auto it=open_documents_copy.begin(); it!=open_documents_copy.end(); it++)
            if (it->reference_count() == 0)
                open_documents_.remove(it.key());
        ml.unlock();

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // save documents
        for (auto it=open_documents_copy.begin(); it!=open_documents_copy.end(); it++) {
            const Document& document = it.key();
            OpenDocument& open_document = it.value();

            QList<Symbol> text = open_document.text();
            QString document_owner = document.owner();
            QString document_name = document.name();

            // delete old document text
            query = query_delete_document_text(database, document);
            execute_query(query);

            // insert updated document text
            query = prepare_query_insert_character(database, document);
            for (int i=0; i<text.size(); i++) {
                QString username = open_document.username(text[i].site_id());
                QChar value = text[i].value();
                bind_query_insert_character(query, i, username, value);
                execute_query(query);
            }

            qDebug() << "document saved:" << document.full_name();
        }

        // commit transaction
        database.commit();
    }
}
