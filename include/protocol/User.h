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
#include <QTextCursor>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include "crdt/Symbol.h"

const QString imgPath = ":/images";
namespace collaborative_text_editor {
    class User {
        QTextCursor *cursor_;
        QLabel* cursor_label_;
        QString username_;
        QImage icon_;
        Symbol cursor_position_;

    public:
        User(const QString &username, const Symbol &cursor_position, const QImage &icon = QImage(imgPath + "/user.png"));

        QString username() const;
        QImage icon() const;
        Symbol cursor_position() const;
        int get_cursor_position();
        void setIcon(const QImage &icon);


//        void update_cursor_position(int newCursorPosition, const QTextEdit &editor);


        void init_cursor(QTextEdit *editor, int newCursorPosition);


    };


}
