/*
 * Author: Franco Ruggeri
 */

#include "DocumentManager.h"
#include "IdentityManager.h"
#include "db_utility.h"
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <editor/crdt/SharedEditor.h>
#include <editor/database/DatabaseGuard.h>
#include <editor/database/db_utility.h>

extern IdentityManager identity_manager;

std::optional<editor::DocumentData> DocumentManager::create_document(int session_id, const QString& document_name) {
    // check inputs
    if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");
    if (document_name.isEmpty()) throw std::logic_error("document creation failed: invalid document name");
    QString document_owner = identity_manager.username(session_id);
    editor::Document document(document_owner, document_name);

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);

    // start transaction
    database.transaction();
    QSqlQuery query(database);

    // check if the document already exists
    query.prepare("SELECT * FROM document WHERE owner=:owner AND name=:name FOR UPDATE");
    query.bindValue(":owner", QVariant(document_owner));
    query.bindValue(":name", QVariant(document_name));
    editor::execute_query(query);

    qDebug() << "select done";

    // create document
    std::optional<editor::DocumentData> document_data;
    if (query.size() == 0) {
        document_data = editor::DocumentData(editor::SharedEditor::starting_site_id,
                                             editor::SharedEditor::starting_site_counter);

        // insert document in DB
        query.prepare("INSERT INTO document (owner, name, sharing_link) "
                      "VALUES (:owner, :name, :sharing_link)");
        query.bindValue(":owner", QVariant(document_owner));
        query.bindValue(":name", QVariant(document_name));
        query.bindValue(":sharing_link", document_data->sharing_link());
        editor::execute_query(query);

        qDebug() << "insert document done";

        // insert sharing in DB
        query.prepare("INSERT INTO sharing (sharing_user, document_owner, document_name, site_id, site_counter"
                      ":sharing_user, :document_owner, :document_name, :site_id, :site_counter");
        query.bindValue(":sharing_user", document_owner);
        query.bindValue(":document_owner", document_owner);
        query.bindValue("document_name", document_name);
        query.bindValue("site_id", document_data->site_id_user());
        query.bindValue("site_counter", document_data->site_counter_user());

        qDebug() << "insert sharing done";

        // insert in memory
        QMutexLocker ml(&mutex_);
        auto local_copy = QSharedPointer<editor::SafeSharedEditor>::create(editor::SharedEditor::server_site_id,
                                                                           editor::SharedEditor::invalid_site_counter);
        local_copies_.insert(document, local_copy);
        open_documents_[session_id].insert(document);
    }

    // commit transaction
    database.commit();

    return document_data;
}

QSharedPointer<editor::SafeSharedEditor> DocumentManager::local_copy(const editor::Document& document) {
    QMutexLocker ml(&mutex_);
    auto it = local_copies_.find(document);
    if (it == local_copies_.end()) throw std::logic_error("insertion failed: file not opened");
    return *it;
}

void DocumentManager::insert_symbol(const editor::Document& document, const editor::Symbol& symbol) {
    local_copy(document)->remote_insert(symbol);
}

void DocumentManager::erase_symbol(const editor::Document& document, const editor::Symbol& symbol) {
    local_copy(document)->remote_erase(symbol);
}

void DocumentManager::save() {
    // TODO
}
