/*
 * Authors: Antonino Musmeci,Franco Ruggeri
 */

#include <client/texteditor.h>
#include "protocol/User.h"
#include <QRect>
namespace collaborative_text_editor {
    User::User(const QString &username, const Symbol &cursor_position, const QImage &icon) : username_(username), icon_(icon),
                                                                                             cursor_position_(cursor_position){


    }

    QImage User::icon() const {
        return icon_;
    }

    QString User::username() const {
        return username_;
    }

    Symbol User::cursor_position() const {
        return cursor_position_;
    }

    void User::setIcon(const QImage &icon) {
        icon_ = icon;
    }

    void User::init_cursor(QTextEdit *editor,int newCursorPosition){
        cursor_ = new QTextCursor(editor->document());
        newCursorPosition = qMin(newCursorPosition, editor->toPlainText().size());
        cursor_->setPosition(newCursorPosition);
        cursor_label_ = new QLabel(editor);
        QPixmap pix(3, 24);
        pix.fill(Qt::yellow);
        cursor_label_ ->setPixmap(pix);

        const QRect qRect = editor->cursorRect(*cursor_);


        cursor_label_->move(qRect.left(), qRect.top());
        cursor_label_->show();
    }

    int User::get_cursor_position() {
        return 4;
    }

//    void User::updateCursorPosition(int newCursorPosition, const  QTextEdit & editor) {
//        newCursorPosition = qMin(newCursorPosition, editor.toPlainText().size());
//        userCursor->setPosition(newCursorPosition); // posizione cursore user.userCursor->
//        const QRect qRect = editor.cursorRect(*userCursor);
//        userCursorLabel->move(qRect.left(), qRect.top());
//        userCursorLabel->show();
//
//    }



}