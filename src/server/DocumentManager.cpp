/*
 * Author: Franco Ruggeri
 */

#include "DocumentManager.h"
#include <database/db_utility_secret.h>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <cte/crdt/SharedEditor.h>
#include <cte/database/DatabaseGuard.h>
#include <cte/database/db_utility.h>

std::optional<cte::DocumentData> DocumentManager::create_document(int session_id,
                                                                  const cte::Document& document) {
    // check inputs
    if (document.name().isEmpty()) throw std::logic_error("document creation failed: invalid document name");

    // open connection
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);

    // start transaction
    database.transaction();
    QSqlQuery query(database);

    // check if the document already exists
    query = query_select_document(database, document);
    cte::execute_query(query);

    // create document
    std::optional<cte::DocumentData> document_data;
    if (query.size() == 0) {
        document_data = cte::DocumentData(cte::SharedEditor::starting_site_id, document.generate_sharing_link());

        // insert in DB
        query = query_insert_document(database, document, document_data->sharing_link());
        cte::execute_query(query);

        // insert in memory
        QMutexLocker ml(&mutex_);
        auto local_copy = QSharedPointer<cte::SafeSharedEditor>::create(cte::SharedEditor::invalid_site_id);
        open_documents_.insert(document, {local_copy, 1});
        site_ids_[session_id].insert(document, document_data->site_id_user());
    }

    // commit transaction
    database.commit();

    return document_data;
}

std::optional<cte::DocumentData> DocumentManager::open_document(int session_id, const QString &username,
                                                                const cte::Document &document) {
//    // check inputs
//    if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");
//    if (document_name.isEmpty()) throw std::logic_error("document creation failed: invalid document name");
//    QString document_owner = identity_manager.username(session_id);
//    cte::Document document(document_owner, document_name);
//
//    // open connection
//    QSqlDatabase database = connect_to_database();
//    cte::DatabaseGuard dg(database);
//
//    // start transaction
//    database.transaction();
//    QSqlQuery query(database);
//
//    // check if the document already exists
//    query.prepare("SELECT * FROM document WHERE owner=:owner AND name=:name FOR UPDATE");
//    query.bindValue(":owner", QVariant(document_owner));
//    query.bindValue(":name", QVariant(document_name));
//    cte::execute_query(query);
//
//    // create document
//    std::optional<cte::DocumentData> document_data;
//    if (query.size() == 0) {
//        document_data = cte::DocumentData(cte::SharedEditor::starting_site_id,
//                                             cte::SharedEditor::starting_site_counter);
//
//        // insert document in DB
//        query.prepare("INSERT INTO document (owner, name, sharing_link) "
//                      "VALUES (:owner, :name, :sharing_link)");
//        query.bindValue(":owner", QVariant(document_owner));
//        query.bindValue(":name", QVariant(document_name));
//        query.bindValue(":sharing_link", document_data->sharing_link());
//        cte::execute_query(query);
//
//        // insert sharing in DB
//        query.prepare("INSERT INTO sharing (sharing_user, document_owner, document_name, site_id, site_counter"
//                      ":sharing_user, :document_owner, :document_name, :site_id, :site_counter");
//        query.bindValue(":sharing_user", document_owner);
//        query.bindValue(":document_owner", document_owner);
//        query.bindValue("document_name", document_name);
//        query.bindValue("site_id", document_data->site_id_user());
//        query.bindValue("site_counter", document_data->site_counter_user());
//
//        // insert in memory
//        QMutexLocker ml(&mutex_);
//        auto local_copy = QSharedPointer<cte::SafeSharedEditor>::create(cte::SharedEditor::server_site_id,
//                                                                           cte::SharedEditor::invalid_site_counter);
//        local_copies_.insert(document, local_copy);
//        open_documents_[session_id].insert(document);
//    }
//
//    // commit transaction
//    database.commit();
//
//    return document_data;
    return std::nullopt;
}

QSharedPointer<cte::SafeSharedEditor> DocumentManager::local_copy(const cte::Document& document) {
    QMutexLocker ml(&mutex_);
    auto it = open_documents_.find(document);
    if (it == open_documents_.end()) throw std::logic_error("file not opened");
    return it->local_copy;
}

bool DocumentManager::site_id_spoofing(int session_id, const cte::Document& document,
                                       const cte::Symbol& symbol) {
    // find session site_ids
    auto it_session_site_ids = site_ids_.find(session_id);
    if (it_session_site_ids == site_ids_.end()) throw std::logic_error("file not opened");
    QHash<cte::Document,int>& session_site_ids = *it_session_site_ids;

    // find document site_id
    auto it_site_id = session_site_ids.find(document);
    if (it_site_id == session_site_ids.end()) throw std::logic_error("file not opened");
    int& site_id = *it_site_id;

    return site_id != symbol.site_id();
}

void DocumentManager::insert_symbol(int session_id, const cte::Document& document, const cte::Symbol& symbol) {
    if (site_id_spoofing(session_id, document, symbol)) throw std::logic_error("site_id spoofing");
    local_copy(document)->remote_insert(symbol);
}

void DocumentManager::erase_symbol(const cte::Document& document, const cte::Symbol& symbol) {
    local_copy(document)->remote_erase(symbol);
}

void DocumentManager::save() {
    // TODO
}
