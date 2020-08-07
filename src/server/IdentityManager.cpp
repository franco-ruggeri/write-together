/*
 * Author: Franco Ruggeri
 */

#include "include/IdentityManager.h"
#include "../database/include/db_utility_secret.h"
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <cte/database/DatabaseGuard.h>
#include <cte/database/db_utility.h>
#include <bcrypt/BCrypt.hpp>

IdentityManager::IdentityManager() : m_sessions_(QMutex::Recursive) {}

bool IdentityManager::signup(int session_id, const cte::Profile& profile, const QString& password) {
    // check inputs
    QString username = profile.username();
    if (authenticated(session_id)) throw std::logic_error("session already authenticated");
    if (username.isEmpty() || password.isEmpty()) throw std::logic_error("signup failed: invalid credentials");

    // open connection and start transaction
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    database.transaction();
    QSqlQuery query(database);

    // check if the username is already used
    query = query_select_profile_for_update(database, username);
    cte::execute_query(query);

    // signup
    bool signed_up = false;
    if (query.size() == 0) {
        // insert profile
        QString hash = QString::fromStdString(BCrypt::generateHash(password.toStdString()));
        query = query_insert_profile(database, profile, hash);
        cte::execute_query(query);

        // authenticate session
        QMutexLocker ml(&m_sessions_);
        sessions_.insert(session_id, username);
        signed_up = true;
    }

    // commit transaction
    database.commit();

    return signed_up;
}

std::optional<cte::Profile> IdentityManager::login(int session_id, const QString& username, const QString& password) {
    // check inputs
    if (authenticated(session_id)) throw std::logic_error("session already authenticated");

    // open connection
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    QSqlQuery query(database);

    // load profile
    query = query_select_profile(database, username);
    cte::execute_query(query);
    if (!query.next()) return std::nullopt;    // non-existing user

    // check password
    QString hash = query.value("password").toString();
    if (!BCrypt::validatePassword(password.toStdString(), hash.toStdString()))
        return std::nullopt;    // wrong password

    // authenticate session
    QMutexLocker ml(&m_sessions_);
    sessions_.insert(session_id, username);

    return cte::Profile(username, query.value("name").toString(), query.value("surname").toString(),
                        query.value("icon").value<QImage>());
}

void IdentityManager::logout(int session_id) {
    // remove session from authenticated ones
    QMutexLocker ml(&m_sessions_);
    if (!authenticated(session_id)) throw std::logic_error("session not authenticated");
    sessions_.remove(session_id);
}

bool IdentityManager::update_profile(int session_id, const cte::Profile& new_profile, const QString& new_password) {
    // check inputs
    if (!authenticated(session_id)) throw std::logic_error("session not authenticated");
    QString old_username = *username(session_id);
    QString new_username = new_profile.username();
    if (new_username.isEmpty() || (!new_password.isNull() && new_password.isEmpty())) return false;

    // open connection and start transaction
    QSqlDatabase database = connect_to_database();
    cte::DatabaseGuard dg(database);
    database.transaction();
    QSqlQuery query(database);

    // check if the new username is already used
    query = query_select_profile_for_update(database, new_username);
    cte::execute_query(query);

    // update profile
    bool updated = false;
    if (query.size() == 0) {
        // update profile
        if (new_password.isNull())
            query = query_update_profile(database, old_username, new_profile);
        else {
            QString new_hash = QString::fromStdString(BCrypt::generateHash(new_password.toStdString()));
            query = query_update_profile(database, old_username, new_profile, new_hash);
        }
        cte::execute_query(query);

        // update session
        QMutexLocker ml(&m_sessions_);
        sessions_[session_id] = new_username;
        updated = true;
    }

    // commit transaction
    database.commit();

    return updated;
}

bool IdentityManager::authenticated(int session_id) const {
    QMutexLocker ml(&m_sessions_);
    return sessions_.contains(session_id);
}

std::optional<QString> IdentityManager::username(int session_id) const {
    QMutexLocker ml(&m_sessions_);
    auto it = sessions_.find(session_id);
    if (it == sessions_.end()) return std::nullopt;
    return *it;
}
