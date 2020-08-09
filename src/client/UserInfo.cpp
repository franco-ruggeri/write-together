/*
 * Author: Antonino Musmeci
 */

#include "UserInfo.h"
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>


    UserInfo::UserInfo(){}


    UserInfo::UserInfo(Profile profile): username_(profile.username()), name_(profile.name()),surname_(profile.surname()),icon_(profile.icon()){

    }


    QString UserInfo::username() const {
        return username_;
    }

    QString UserInfo::name() const {
        return name_;
    }

    QString UserInfo::surname() const {
        return surname_;
    }

    QImage UserInfo::icon() const {
        return icon_;
    }


    //cursors

    void UserInfo::init_cursor(QTextEdit *editor, int newCursorPosition) {
        cursor_ = new QTextCursor(editor->document());
        newCursorPosition = qMin(newCursorPosition, editor->toPlainText().size());
        cursor_->setPosition(newCursorPosition);
        cursor_label_ = new QLabel(editor);
        QPixmap pix(3, 24);
        pix.fill(Qt::yellow);
        cursor_label_->setPixmap(pix);

        const QRect qRect = editor->cursorRect(*cursor_);
        cursor_label_->move(qRect.left(), qRect.top());
        cursor_label_->show();
    }


    void UserInfo::change_cursor_position(QTextEdit *editor, int new_position) {
        new_position = qMin(new_position, editor->toPlainText().size());
        cursor_->setPosition(new_position);
        const QRect qRect = editor->cursorRect(*cursor_);
        cursor_label_->move(qRect.left(), qRect.top());
        cursor_label_->show();

    }

void UserInfo::setIcon(const QImage &icon) {

}

