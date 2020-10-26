/*
 * User profile. It holds all the information about a user and is used both by server and client.
 *
 * Notice that neither the password nor its hash is stored here. This is intentional, because the profile is thought
 * to be sent to users other than the owner. The password hash must stored only in the server host (file or DB),
 * protected with access control, and used just for authentication.
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>

namespace cte {
    class Profile {
        QString username_, name_, surname_, email_;
        QImage icon_;
        static const char *icon_format_;

    public:
        Profile();
        Profile(const QString& username, const QString& name, const QString& surname, const QString& email,
                const QImage& icon);
        Profile(const QString& username, const QString& name, const QString& surname, const QString& email,
                const QByteArray& icon);
        explicit Profile(const QJsonObject& json_object);

        bool operator==(const Profile& other) const;

        QString username() const;
        QString name() const;
        QString surname() const;
        QString email() const;
        QImage icon() const;
        QByteArray icon_data() const;
        void set_icon(const QByteArray& data);
        void set_icon(const QImage& data);

        bool valid_username() const;
        bool valid_name() const;
        bool valid_surname() const;
        bool valid_email() const;

        QJsonObject json() const;

        static bool valid_username(const QString& username);
        static bool valid_password(const QString& password);
    };


}
