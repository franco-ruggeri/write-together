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
#include <QtGui/QTextCursor>
#include <QtWidgets/QLabel>
#include <crdt/Symbol.h>
#include <QtWidgets/QTextEdit>
#include "Document.h"

namespace collaborative_text_editor {
    class Profile {
        QString username_, name_, surname_;
        QImage icon_;


//cursor
        QTextCursor *cursor_;
        QLabel* cursor_label_;
        Symbol cursor_position_;

    public:
        Profile();
        Profile(const QString& username, const QString& name, const QString& surname);
        Profile(const QString& username, const QString& name, const QString& surname, const QImage& icon);
        Profile(const QJsonObject& json_object);

        bool operator==(const Profile& other) const;

        QString username() const;
        QString name() const;
        QString surname() const;
        QImage icon() const;

        QJsonObject json() const;



        //cursors
        int get_cursor_position();
        void setIcon(const QImage &icon);
        QMap<QString,Document> filename_to_owner_map;
        void init_cursor(QTextEdit *editor, int newCursorPosition);
    };
}
