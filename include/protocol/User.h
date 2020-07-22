/*
 * User profile. It holds all the information about a user and is used both by server and client.
 *
 * Notice that the password is intentionally not stored here. Its hash is stored in a file and is loaded and used just
 * for authentication. The password file should be protected with access control.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QImage>

namespace collaborative_text_editor {
    class User {
        QString username_;
        QPixmap icon_;

    public:
        User(QString username, QImage icon);
        QString username() const;
        QImage icon() const;
    };
}
