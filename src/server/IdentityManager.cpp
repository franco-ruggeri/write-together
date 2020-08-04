/*
 * Author: Franco Ruggeri
 */

#include "IdentityManager.h"
#include "db_utility.h"
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <editor/database/DatabaseGuard.h>
#include <editor/database/db_utility.h>
#include <bcrypt/BCrypt.hpp>

bool IdentityManager::signup(const editor::Profile& profile, const QString& password) {
    // check inputs
    QString username = profile.username();
    if (username.isEmpty() || password.isEmpty())
        throw std::logic_error("invalid credentials");

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);

    // start transaction
    database.transaction();
    QSqlQuery query(database);

    // check if the username is already used
    query.prepare("SELECT * FROM user WHERE username=:username FOR UPDATE");
    query.bindValue(":username", QVariant(username));
    editor::execute_query(query);

    // signup
    bool signed_up = false;
    if (query.size() == 0) {
        QString hash = QString::fromStdString(BCrypt::generateHash(password.toStdString()));
        query.prepare("INSERT INTO user (username, password, name, surname, icon) "
                      "VALUES (:username, :password, :name, :surname, :icon)");
        query.bindValue(":username", QVariant(username));
        query.bindValue(":password", QVariant(hash));
        query.bindValue(":name", QVariant(profile.name()));
        query.bindValue(":surname", QVariant(profile.surname()));
        query.bindValue(":icon", QVariant(profile.icon()));
        editor::execute_query(query);
        signed_up = true;
    }

    // commit transaction
    database.commit();

    return signed_up;
}

std::optional<editor::Profile> IdentityManager::login(const QString& username, const QString& password) {
    std::optional<editor::Profile> profile;

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);
    QSqlQuery query(database);

    // load profile
    query.prepare("SELECT * FROM user WHERE username=:username");
    query.bindValue(":username", QVariant(username));
    editor::execute_query(query);

    // login
    if (query.next()) {
        QString hash = query.value("password").toString();
        if (BCrypt::validatePassword(password.toStdString(), hash.toStdString()))
            profile = editor::Profile(username, query.value("name").toString(), query.value("surname").toString(),
                                      query.value("icon").value<QImage>());
    }

    return profile;
}

bool IdentityManager::update_profile(const QString& old_username, const editor::Profile& new_profile,
                                     const QString& new_password) {
    // check inputs
    QString new_username = new_profile.username();
    if (new_username.isEmpty() || (!new_password.isNull() && new_password.isEmpty()))
        return false;

    // open connection
    QSqlDatabase database = connect_to_database();
    editor::DatabaseGuard dg(database);

    // start transaction
    database.transaction();
    QSqlQuery query(database);

    // check if the new username is already used
    query.prepare("SELECT * FROM user WHERE username=:username FOR UPDATE");
    query.bindValue(":username", QVariant(new_username));
    editor::execute_query(query);

    // update profile
    bool updated = false;
    if (query.size() == 0) {
        bool update_password = new_password.isNull();
        QString query_string = QString{} + "UPDATE user SET username=:username, " +
                               (update_password ? "password=:password, " : "") +
                               "name=:name, surname=:surname, icon=:icon WHERE username=:old_username";
        query.prepare(query_string);
        query.bindValue(":old_username", QVariant(old_username));
        query.bindValue(":username", QVariant(new_username));
        query.bindValue(":name", QVariant(new_profile.name()));
        query.bindValue(":surname", QVariant(new_profile.surname()));
        query.bindValue(":icon", QVariant(new_profile.icon()));
        if (update_password) {
            QString new_hash = QString::fromStdString(BCrypt::generateHash(new_password.toStdString()));
            query.bindValue(":password", new_hash);
        }
        editor::execute_query(query);
        updated = true;
    }

    // commit transaction
    database.commit();

    return updated;
}
