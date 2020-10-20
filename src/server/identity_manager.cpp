/*
 * Author: Franco Ruggeri
 */

#include <cte/server/identity_manager.h>
#include <cte/database/database_guard.h>
#include <cte/database/database_utility.h>
#include <cte/crypto/password_utility.h>
#include <database_utility_secret.h>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <optional>

namespace cte {
    IdentityManager::IdentityManager() : m_sessions_(QMutex::Recursive) {}

    bool IdentityManager::signup(int session_id, const Profile& profile, QString&& password) {
        // check inputs
        QString username = profile.username();
        if (authenticated(session_id)) throw std::logic_error("session already authenticated");
        if (!Profile::check_username(username)) throw std::logic_error("signup failed: invalid username");
        if (!Profile::check_email(profile.email())) throw std::logic_error("signup failed: invalid email");
        if (!Profile::check_password(password)) throw std::logic_error("signup failed: invalid password");

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the username is already used
        query = query_select_profile(database, username, true);
        execute_query(query);

        // signup
        bool signed_up = false;
        if (query.size() == 0) {
            // insert profile
            QString hash = QString::fromStdString(generate_password(static_cast<QString &&>(password)));
            query = query_insert_profile(database, profile, hash);
            execute_query(query);

            // authenticate session
            QMutexLocker ml(&m_sessions_);
            sessions_.insert(session_id, username);
            signed_up = true;
        }

        // commit transaction
        database.commit();

        return signed_up;
    }

    std::optional<Profile> IdentityManager::login(int session_id, const QString& username, QString&& password) {
        // check inputs
        if (authenticated(session_id)) throw std::logic_error("session already authenticated");

        // open connection
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        QSqlQuery query(database);

        // load profile
        query = query_select_profile(database, username);
        execute_query(query);
        if (!query.next()) return std::nullopt;    // non-existing user

        // check password
        QString hash = query.value("password").toString();
        if (!verify_password(static_cast<QString &&>(password), hash.toStdString()))
            return std::nullopt;    // wrong password

        // authenticate session
        QMutexLocker ml(&m_sessions_);
        sessions_.insert(session_id, username);

        return Profile(username, query.value("name").toString(), query.value("surname").toString(),
                       query.value("email").toString(), query.value("icon").toByteArray());
    }

    void IdentityManager::logout(int session_id) {
        // remove session from authenticated ones
        QMutexLocker ml(&m_sessions_);
        if (!authenticated(session_id)) throw std::logic_error("session not authenticated");
        sessions_.remove(session_id);
    }

    void IdentityManager::update_profile(int session_id, const Profile& new_profile, QString&& new_password) const {
        // check inputs
        if (!authenticated(session_id)) throw std::logic_error("session not authenticated");
        if (new_profile.username() != *username(session_id))
            throw std::logic_error("profile update failed: username cannot be changed");
        if (!Profile::check_email(new_profile.email()))
            throw std::logic_error("profile update failed: invalid email");
        if (!new_password.isNull() && !Profile::check_password(new_password))
            throw std::logic_error("profile update failed: invalid password");

        // open connection
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        QSqlQuery query(database);

        // update profile
        if (new_password.isNull())
            query = query_update_profile(database, new_profile);
        else {
            QString new_hash = QString::fromStdString(
                    generate_password(static_cast<QString &&>(new_password)));
            query = query_update_profile(database, new_profile, new_hash);
        }
        execute_query(query);
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
}
