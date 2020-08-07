/*
 * Author: Franco Ruggeri
 */

#include <DocumentManager.h>
#include <db_utility_secret.h>
#include <QtCore/QVariant>
#include <QtCore/QSet>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <cte/crdt/SharedEditor.h>
#include <cte/database/DatabaseGuard.h>
#include <cte/database/db_utility.h>

DocumentManager::DocumentManager() : mutex_(QMutex::Recursive) {}

std::optional<cte::DocumentData> DocumentManager::create_document(int session_id,
                                                                  const cte::Document& document) {
    // check inputs
    if (document.name().isEmpty()) throw std::logic_error("document creation failed: invalid document name");

    // open connection and start transaction
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    database.transaction();
    QSqlQuery query(database);

    // check if the document already exists
    query = query_select_document_for_update(database, document);
    cte::execute_query(query);

    // create document
    std::optional<cte::DocumentData> document_data;
    if (query.size() == 0) {
        QString owner = document.owner();
        QUrl sharing_link = cte::Document::generate_sharing_link(document);

        // insert new document
        query = query_insert_document(database, document, sharing_link);
        cte::execute_query(query);
        query = query_insert_sharing(database, document, owner);
        cte::execute_query(query);

        // open document
        OpenDocument od;
        int site_id = od.open(owner);
        QMutexLocker ml(&mutex_);
        open_documents_.insert(document, od);
        site_ids_[session_id].insert(document, document_data->site_id());
        document_data = cte::DocumentData(site_id, sharing_link);
    }

    // commit transaction
    database.commit();

    return document_data;
}

std::optional<cte::DocumentData> DocumentManager::open_document(int session_id, const cte::Document& document,
                                                                const QString& username) {
    // check inputs
    if (opened(session_id, document)) throw std::logic_error("document already opened");

    // open connection and start transaction
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    database.transaction();
    QSqlQuery query(database);

    // check if the document is accessible by the user (selecting also sharing link)
    query = query_select_shared_document(database, document, username);
    cte::execute_query(query);

    // open document
    std::optional<cte::DocumentData> document_data;
    if (query.next()) {
        // load sharing link
        QString sharing_link = query.value("sharing_link").toString();

        // load profiles
        query = query_select_document_profiles(database, document);
        QHash<QString,cte::Profile> profiles;
        while (query.next()) {
            QString username = query.value("username").toString();
            cte::Profile profile(username, query.value("name").toString(),
                                 query.value("surname").toString(),
                                 query.value("icon").value<QImage>());
            profiles.insert(username, profile);
        }

        // open document
        QMutexLocker ml(&mutex_);
        if (!open_documents_.contains(document)) {
            // not in main memory... load text
            query = query_select_document_text(database, document);
            cte::execute_query(query);
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
        document_data = cte::DocumentData(od.text(), site_id, od.cursors(), od.site_ids(), profiles, sharing_link);
    }

    // commit transaction
    database.commit();

    return document_data;
}

void DocumentManager::close_document(int session_id, const cte::Document& document) {
    QMutexLocker ml(&mutex_);
    if (!opened(session_id, document)) throw std::logic_error("document not opened");
    int site_id = site_ids_[session_id].take(document);
    get_open_document(document).close(site_id);
}

void DocumentManager::close_documents(int session_id) {
    QMutexLocker ml(&mutex_);
    auto it_sessions = site_ids_.find(session_id);
    if (it_sessions == site_ids_.end()) return;  // no documents opened
    QHash<cte::Document,int> session_site_ids = *it_sessions;
    for (auto it=session_site_ids.begin(); it != session_site_ids.end(); it++)
        open_documents_[it.key()].close(it.value());
    site_ids_.remove(session_id);
}

OpenDocument& DocumentManager::get_open_document(const cte::Document& document) {
    auto it = open_documents_.find(document);
    if (it == open_documents_.end()) throw std::logic_error("document not opened");
    return *it;
}

bool DocumentManager::opened(int session_id, const cte::Document& document) const {
    QMutexLocker ml(&mutex_);
    auto it = site_ids_.find(session_id);
    return it != site_ids_.end() && it->contains(document);
}

bool DocumentManager::site_id_spoofing(int session_id, const cte::Document& document, const cte::Symbol& symbol) const {
    QMutexLocker ml(&mutex_);
    return opened(session_id, document) && site_ids_[session_id][document] == symbol.site_id();
}

void DocumentManager::insert_symbol(int session_id, const cte::Document& document, const cte::Symbol& symbol) {
    QMutexLocker ml(&mutex_);
    if (!opened(session_id, document)) throw std::logic_error("document not opened");
    if (site_id_spoofing(session_id, document, symbol)) throw std::logic_error("site_id spoofing");
    get_open_document(document).insert_symbol(symbol);
}

void DocumentManager::erase_symbol(int session_id, const cte::Document& document, const cte::Symbol& symbol) {
    QMutexLocker ml(&mutex_);
    if (!opened(session_id, document)) throw std::logic_error("document not opened");
    get_open_document(document).erase_symbol(symbol);
}

void DocumentManager::move_cursor(int session_id, const cte::Document& document, const cte::Symbol& symbol) {
    QMutexLocker ml(&mutex_);
    if (!opened(session_id, document)) throw std::logic_error("document not opened");
    get_open_document(document).move_cursor(site_ids_[session_id][document], symbol);
}

cte::Document DocumentManager::document(const QUrl &sharing_link) const {
    // open connection
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    QSqlQuery query(database);

    // load document
    query = query_select_document(database, sharing_link);
    if (!query.next()) throw std::logic_error("invalid sharing link");
    return cte::Document(query.value("owner").toString(), query.value("name").toString());
}

QSet<cte::Document> DocumentManager::documents(int session_id, const QString& username) const {
    // open connection
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    QSqlQuery query(database);

    // load accessible documents
    query = query_select_shared_documents(database, username);
    QSet<cte::Document> documents;
    while (query.next()) {
        cte::Document document(query.value("document_owner").toString(), query.value("document_name").toString());
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

void DocumentManager::save() const {
    // TODO
}
