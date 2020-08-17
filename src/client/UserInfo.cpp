/*
 * Author: Antonino Musmeci
 */

#include "UserInfo.h"
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>
#include <QDebug>

    UserInfo::UserInfo(){}
UserInfo::UserInfo(Profile profile): username_(profile.username()), name_(profile.name()),surname_(profile.surname()),icon_(profile.icon()){}

    UserInfo::UserInfo(Profile profile, QColor color): username_(profile.username()), name_(profile.name()),surname_(profile.surname()),icon_(profile.icon()),
    color_(color),selected(false){}


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


void  UserInfo::draw_background_char(QTextEdit *editor,int start, int end) {

    QTextCharFormat format;
    if(selected )
        format.setBackground(Qt::transparent);
    else
        format.setBackground(color_);

    format.setFontPointSize(14);
    QTextCursor cursor(editor->document());
    cursor.setPosition(start);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(format);

}

void UserInfo::setIcon(const QImage &icon) {

}

const QColor UserInfo::color() {
    return color_;
}


void UserInfo::add_cursor(QTextEdit *editor, int newCursorPosition, int id){
    QLabel* cursor_label_= new QLabel(editor);
    cursor_ = new QTextCursor(editor->document());
    newCursorPosition = qMin(newCursorPosition, editor->toPlainText().size());
    cursor_->setPosition(newCursorPosition);
    QPixmap pix(3, 24);
    pix.fill(color_);
    cursor_label_->setPixmap(pix);
    const QRect qRect = editor->cursorRect(*cursor_);
    cursor_label_->move(qRect.left(), qRect.top());
    site_id_to_cursor.insert(id,cursor_label_);
    cursor_label_->show();
}


void UserInfo::change_cursor_position(QTextEdit *editor, int new_position, int id) {
    qDebug() << username();
    qDebug() << new_position;
    new_position = qMin(new_position, editor->toPlainText().size());
    cursor_->setPosition(new_position);
    const QRect qRect = editor->cursorRect(*cursor_);
    QLabel *cursor_label_ = site_id_to_cursor.find(id).value();
    cursor_label_->move(qRect.left(), qRect.top());
    cursor_label_->show();

}