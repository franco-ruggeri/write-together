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
        QString owner = document.owner();
        bool created = false;
        query = query_select_document(database, document, true);
        execute_query(query);
        if (query.size() == 0) {
            // create document
            QUrl sharing_link = Document::generate_sharing_link(document);
            query = query_insert_document(database, document, sharing_link);
            execute_query(query);
            query = query_insert_sharing(database, document, owner);
            execute_query(query);
            created = true;
        }

        // commit transaction
        database.commit();

        // open document
        return created ? open_document(session_id, document, owner) : std::nullopt;
    }

    std::optional<DocumentInfo>
    DocumentManager::open_document(int session_id, const Document& document, const QString& username) {
        // check inputs
        if (opened(session_id, document)) throw std::logic_error("document already opened");

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the document is accessible by the user
        QHash<QString,Profile> profiles;
        QString sharing_link;
        bool accessible = false;
        query = query_select_document(database, document, username);
        execute_query(query);
        if (query.next()) {     // accessible
            sharing_link = query.value("sharing_link").toString();
            accessible = true;

            // load profiles
            query = query_select_document_profiles(database, document);
            execute_query(query);
            while (query.next()) {
                Profile profile(query.value("username").toString(),
                                query.value("name").toString(),
                                query.value("surname").toString(),
                                query.value("email").toString(),
                                query.value("icon").toByteArray());
                profiles.insert(profile.username(), profile);
            }
        }

        // commit transaction (important: before opening the document because OpenDocument uses the DB)
        database.commit();
        if (!accessible) return std::nullopt;

        // open document (if not already opened)
        QMutexLocker ml(&mutex_);
        if (!open_documents_.contains(document))
            open_documents_.insert(document, OpenDocument(document));
        OpenDocument& od = *open_documents_.find(document);
        int site_id = od.open(username);
        site_ids_[session_id].insert(document, site_id);
        ml.unlock();

        // prepare document info
        QHash<QString,std::pair<Profile,QList<int>>> users;
        QHash<int,QString> usernames = od.usernames();
        for (const auto& p : profiles)
            users.insert(p.username(), {p, QList<int>()} );
        for (auto it=usernames.begin(); it != usernames.end(); it++)
            users[it.value()].second.append(it.key());
        return DocumentInfo(od.text(), site_id, od.cursors(), users, sharing_link);
    }

    std::optional<std::pair<Document,DocumentInfo>>
    DocumentManager::open_document(int session_id, const QUrl& sharing_link, const QString& username) {
        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the document exists
        std::optional<Document> document;
        query = query_select_document(database, sharing_link);
        execute_query(query);
        if (query.next()) {
            document = Document(query.value("owner").toString(), query.value("name").toString());

            // add sharing if not present
            query = query_select_document(database, *document, username);
            execute_query(query);
            if (query.size() == 0) {
                query = query_insert_sharing(database, *document, username);
                execute_query(query);
            }
        }

        // commit transaction
        database.commit();

        // open document
        if (document)
            return std::make_pair(*document, *open_document(session_id, *document, username));
        else
            return std::nullopt;
    }

    int DocumentManager::close_document(int session_id, const Document& document) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id].take(document);
        OpenDocument& od = get_open_document(document);
        ml.unlock();    // release here, other threads can work on other documents!
        od.close(site_id);
        return site_id;
    }

    OpenDocument& DocumentManager::get_open_document(const Document& document) {
        QMutexLocker ml(&mutex_);
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

    void DocumentManager::insert_symbol(int session_id, const Document& document, const Symbol& symbol,
                                        const Format& format) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        if (site_id_spoofing(session_id, document, symbol)) throw std::logic_error("site_id spoofing");
        OpenDocument& od = get_open_document(document);
        ml.unlock();    // release here, other threads can work on other documents!
        od.insert_symbol(symbol, format);
    }

    int DocumentManager::erase_symbol(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id][document];
        OpenDocument& od = get_open_document(document);
        ml.unlock();    // release here, other threads can work on other documents!
        od.erase_symbol(site_id, symbol);
        return site_id;
    }

    int DocumentManager::move_cursor(int session_id, const Document& document, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        int site_id = site_ids_[session_id][document];
        OpenDocument& od = get_open_document(document);
        ml.unlock();    // release here, other threads can work on other documents!
        od.move_cursor(site_id, symbol);
        return site_id;
    }

    void DocumentManager::format_symbol(int session_id, const Document& document, const Symbol& symbol,
                                        const Format& format) {
        QMutexLocker ml(&mutex_);
        if (!opened(session_id, document)) throw std::logic_error("document not opened");
        OpenDocument& od = get_open_document(document);
        ml.unlock();    // release here, other threads can work on other documents!
        od.format_symbol(symbol, format);
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
        // copy (so that we do not lock for the entire, slow, saving on the DB)
        QMutexLocker ml(&mutex_);
        QHash<Document,OpenDocument> open_documents_copy = open_documents_;
        for (auto it=open_documents_copy.begin(); it!=open_documents_copy.end(); it++)
            if (it->reference_count() == 0)     // closed, can be removed from the open documents
                open_documents_.remove(it.key());
        ml.unlock();

        // save
        for (auto& od : open_documents_copy)
            od.save();
    }
}
