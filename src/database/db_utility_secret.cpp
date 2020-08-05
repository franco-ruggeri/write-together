/*
 * Author: Franco Ruggeri
 */

#include "db_utility_secret.h"
#include <QtCore/QVariant>
#include <cte/database/db_utility.h>

static const QString driver_type("QMYSQL");
static const QString database_name("collaborative_text_editor");
static const QString hostname("127.0.0.1");
static const QString username("collaborative_text_editor");
static const QString password("?PdSPr0j3ct!");

QSqlDatabase connect_to_database() {
    return cte::connect_to_database(driver_type, database_name, hostname, username, password);
}

QSqlQuery query_select_profile(const QSqlDatabase& database, const QString& username) {
    QSqlQuery query(database);
    query.prepare("SELECT * FROM user WHERE username=:username FOR UPDATE");
    query.bindValue(":username", QVariant(username));
    return query;
}

QSqlQuery query_insert_profile(const QSqlDatabase& database, const cte::Profile& profile, const QString& password) {
    QSqlQuery query(database);
    query.prepare("INSERT INTO user (username, password, name, surname, icon) "
                  "VALUES (:username, :password, :name, :surname, :icon)");
    query.bindValue(":username", QVariant(profile.username()));
    query.bindValue(":password", QVariant(password));
    query.bindValue(":name", QVariant(profile.name()));
    query.bindValue(":surname", QVariant(profile.surname()));
    query.bindValue(":icon", QVariant(profile.icon()));
    return query;
}

QSqlQuery query_update_profile(const QSqlDatabase& database, const QString& old_username,
                               const cte::Profile& new_profile, const QString& new_password) {
    bool update_password = new_password.isNull();
    QString query_string = QString{} + "UPDATE user SET username=:username, " +
                           (update_password ? "password=:password, " : "") +
                           "name=:name, surname=:surname, icon=:icon WHERE username=:old_username";
    QSqlQuery query(database);
    query.prepare(query_string);
    query.bindValue(":old_username", QVariant(old_username));
    query.bindValue(":username", QVariant(new_profile.username()));
    query.bindValue(":name", QVariant(new_profile.name()));
    query.bindValue(":surname", QVariant(new_profile.surname()));
    query.bindValue(":icon", QVariant(new_profile.icon()));
    if (update_password) query.bindValue(":password", new_password);
    return query;
}

QSqlQuery query_select_document(const QSqlDatabase& database, const cte::Document& document) {
    QSqlQuery query(database);
    query.prepare("SELECT * FROM document WHERE owner=:owner AND name=:name FOR UPDATE");
    query.bindValue(":owner", QVariant(document.owner()));
    query.bindValue(":name", QVariant(document.name()));
    return query;
}

QSqlQuery query_insert_document(const QSqlDatabase& database, const cte::Document& document,
                                const QString& sharing_link) {
    QSqlQuery query(database);
    query.prepare("INSERT INTO document (owner, name, sharing_link) "
                  "VALUES (:owner, :name, :sharing_link)");
    query.bindValue(":owner", QVariant(document.owner()));
    query.bindValue(":name", QVariant(document.name()));
    query.bindValue(":sharing_link", sharing_link);
    return query;
}
