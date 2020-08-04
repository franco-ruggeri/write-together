/*
 * Author: Franco Ruggeri
 */

#include "DocumentManager.h"
#include "db_utility.h"
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <editor/database/DatabaseGuard.h>
#include <editor/database/db_utility.h>

std::optional<editor::DocumentData> DocumentManager::create_document(const editor::Document &document) {
    std::optional<editor::DocumentData> document_data;

    // check inputs
    QString owner = document.owner();
    QString name = document.name();
    if (name.isEmpty())
        throw std::logic_error("invalid credentials");

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);

    // start transaction
    database.transaction();
    QSqlQuery query(database);

    // check if the document already exists
    query.prepare("SELECT * FROM document WHERE owner=:owner AND name=:name FOR UPDATE");
    query.bindValue(":owner", QVariant(owner));
    query.bindValue(":name", QVariant(name));
    editor::execute_query(query);

    // create document
    if (query.size() == 0) {
        // insert in DB
        query.prepare("INSERT INTO document (owner, name, sharing_link) "
                      "VALUES (:username, :owner, :name, :sharing_link)");
        query.bindValue(":owner", QVariant(owner));
        query.bindValue(":name", QVariant(name));
        query.bindValue(":sharing_link", QVariant(""));     // TODO: sharing link
        editor::execute_query(query);

        // insert in memory
        QMutexLocker ml(&mutex_);
        open_documents_[owner].push_back(document);
        cursors_[document].insert(owner, editor::Symbol{});

        // TODO: insert in SHARING

        document_data = editor::DocumentData(); // TODO
    }

    // commit transaction
    database.commit();

    return document_data;
}

std::optional<editor::DocumentData>
DocumentManager::open_document(const editor::Document &document, const QString &username) {
    std::optional<editor::DocumentData> document_data;

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);
    QSqlQuery query(database);

    // load profile
    query.prepare("SELECT * FROM document WHERE owner=:owner AND name=:name");
    query.bindValue(":owner", QVariant(document.owner()));
    query.bindValue(":name", QVariant(document.name()));
    editor::execute_query(query);

    // login
    if (query.next()) {

    }

    // TODO: complete

    return document_data;
}
