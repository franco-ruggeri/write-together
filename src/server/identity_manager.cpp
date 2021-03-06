#include <cte/server/identity_manager.h>
#include <cte/database/database_guard.h>
#include <cte/database/database_utility.h>
#include <cte/crypto/password_utility.h>
#include <database_utility_secret.h>
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
        if (!profile.valid_username()) throw std::logic_error("signup failed: invalid username");
        if (!profile.valid_name()) throw std::logic_error("signup failed: invalid name");
        if (!profile.valid_surname()) throw std::logic_error("signup failed: invalid surname");
        if (!profile.valid_email()) throw std::logic_error("signup failed: invalid email");
        if (!Profile::valid_password(password)) throw std::logic_error("signup failed: invalid password");

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // check if the username is already used
        query = query_select_profile(database, username, true);
        execute_query(query);
        bool signed_up = false;
        if (query.size() == 0) {    // not used

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
        Profile profile(username, query.value("name").toString(), query.value("surname").toString(),
                        query.value("email").toString(), query.value("icon").toByteArray());

        // check password
        QString hash = query.value("password").toString();
        if (!verify_password(std::move(password), hash))
            return std::nullopt;    // wrong password

        // authenticate session
        QMutexLocker ml(&m_sessions_);
        sessions_.insert(session_id, username);

        return profile;
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
        if (!new_profile.valid_name()) throw std::logic_error("profile update failed: invalid name");
        if (!new_profile.valid_surname()) throw std::logic_error("profile update failed: invalid surname");
        if (!new_profile.valid_email()) throw std::logic_error("profile update failed: invalid email");
        if (!new_password.isNull() && !Profile::valid_password(new_password))
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
