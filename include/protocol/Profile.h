/*
 * User profile. It holds all the information about a user and is used both by server and client.
 *
 * Notice that the password is intentionally not stored here. Its hash is stored in a file and is loaded and used just
 * for authentication. The password file should be protected with access control.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>

namespace collaborative_text_editor {
    class Profile {
        QString username_, name_, surname_;
        QImage icon_;

    public:
        Profile();
        Profile(const QString& username);
        Profile(const QString& username, const QString& name, const QString& surname);
        Profile(const QString& username, const QString& name, const QString& surname, const QImage& icon);
        Profile(const QJsonObject& json_object);

        bool operator==(const Profile& other) const;

        QString username() const;
        QString name() const;
        QString surname() const;
        QImage icon() const;

        QJsonObject json() const;
    };
}
