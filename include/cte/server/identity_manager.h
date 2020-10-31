/*
 * Identity manager. It provides an interface for signing up, logging in, logging out, and updating profile.
 *
 * A session is authenticated and bound to a user after signing up or logging in, and not authenticated after logging
 * out.
 *
 * Multi-threading:
 * - It is thread-safe to use different sessions in different threads.
 * - It is not thread-safe to use the same session in different threads.
 */

#pragma once

#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <cte/protocol/profile.h>
#include <optional>

namespace cte {
    class IdentityManager {
        QHash<int,QString> sessions_;     // session_id -> username, for authenticated sessions
        mutable QMutex m_sessions_;

    public:
        IdentityManager();

        bool signup(int session_id, const Profile& profile, QString&& password);
        std::optional<Profile> login(int session_id, const QString& username, QString&& password);
        void logout(int session_id);
        void update_profile(int session_id, const Profile& new_profile, QString&& new_password=QString{}) const;

        bool authenticated(int session_id) const;
        std::optional<QString> username(int session_id) const;
    };
}
