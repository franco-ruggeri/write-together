/*
 * User profile. It holds all the information about a user and is used both by server and client.
 *
 * Notice that neither the password nor its hash is stored here. This is intentional, because the profile is thought
 * to be sent to users other than the owner. The password hash must stored only in the server host (file or DB),
 * protected with access control, and used just for authentication.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>

namespace cte {
    class Profile {
        QString username_, name_, surname_;
        QImage icon_;

    public:
        Profile();
        Profile(const QString& username, const QString& name, const QString& surname, const QImage& icon);
        explicit Profile(const QJsonObject& json_object);

        bool operator==(const Profile& other) const;

        QString username() const;
        QString name() const;
        QString surname() const;
        QImage icon() const;

        QJsonObject json() const;
    };
}
