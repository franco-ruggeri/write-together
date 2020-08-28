/*
 * Author: Antonino Musmeci, Stefano Di Blasio
 */

#include "UserInfo.h"
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>
#include <QDebug>



UserInfo::UserInfo(){}
UserInfo::UserInfo(Profile profile): username_(profile.username()), name_(profile.name()),surname_(profile.surname()),icon_(profile.icon()),selected(false){}

    UserInfo::UserInfo(Profile profile, QColor color): username_(profile.username()), name_(profile.name()),surname_(profile.surname()),icon_(profile.icon()),
    color_(color),selected(false){}

UserInfo& UserInfo::operator=(const Profile &other) {
    username_ = other.username();
    name_ = other.name();
    surname_ = other.surname();
    icon_ = other.icon();
    return *this;
}

void UserInfo::clear_fields() {
    surname_ = name_ = username_ = "";
    icon_ = QImage();
    filename_to_owner_map.clear();
    site_id_to_cursor.clear();
    // maybe other fields to clear
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

QString UserInfo::email() const {
    return email_;
}

QImage UserInfo::icon() const {
    return icon_;
}


    //cursors


void  UserInfo::draw_background_char(QTextEdit *editor,int start, int end) {

    if(selected)
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


void UserInfo::add_cursor(QTextEdit *editor, int new_position, int id){

    auto c = new cursor;
    c->vertical_cursor = new QLabel(editor);
    c->cursor_ = new QTextCursor(editor->document());
    new_position = qMin( new_position, editor->toPlainText().size());
    c->cursor_->setPosition(new_position);
    c->current_position =  new_position;
    QPixmap pix(2, 24);
    pix.fill(color_);
        c->vertical_cursor->setPixmap(pix);
    const QRect qRect = editor->cursorRect(*c->cursor_);
    c->vertical_cursor->move(qRect.left(), qRect.top());
    c->vertical_cursor->show();
    c->horizontal_cursor = new QLabel(editor);
    int x_pos = qRect.center().x();
    int y_pos = qRect.topRight().y() - 4;
    QFontMetrics fm(editor->font());
    QRect bounds = fm.boundingRect(username_);
    c->horizontal_cursor->move(x_pos,y_pos);
    c->horizontal_cursor->setAutoFillBackground(true);
    c->horizontal_cursor->setPalette(color_);
    c->horizontal_cursor->setText(username_);
    site_id_to_cursor.insert(id,c);
    c->horizontal_cursor->show();
    QTextCursor cursor(editor->document());


}


void UserInfo::change_cursor_position(QTextEdit *editor, int new_position, int id) {
    auto c = site_id_to_cursor.find(id).value();
    new_position = qMin(new_position, editor->toPlainText().size());
    if(new_position != c->current_position) {
        c->cursor_->setPosition(new_position);
        c->current_position = new_position;
        const QRect qRect = editor->cursorRect(*c->cursor_);
        QLabel *cursor_label_ = site_id_to_cursor.find(id).value()->vertical_cursor;
        cursor_label_->move(qRect.left(), qRect.top());
        cursor_label_->show();
        auto *horizontal_label = site_id_to_cursor.find(id).value()->horizontal_cursor;
        int x_pos = qRect.center().x();
        int y_pos = qRect.topRight().y() - 4;
        QFontMetrics fm(editor->font());
        QRect bounds = fm.boundingRect(username_);
        horizontal_label->move(x_pos, y_pos);
        horizontal_label->show();
        format.setFontPointSize(14);

    }

}

void UserInfo::remove_cursors(int id){
    QLabel* vertical_cursor= site_id_to_cursor.find(id).value()->vertical_cursor;
    QLabel* horizontal_cursor= site_id_to_cursor.find(id).value()->horizontal_cursor;
    horizontal_cursor->hide();
    vertical_cursor->hide();
    delete horizontal_cursor;
    delete vertical_cursor;
    site_id_to_cursor.remove(id);
}

void UserInfo::update_cursor(QTextEdit *editor){

    for(auto c : site_id_to_cursor){
//        if(c->current_position != c->cursor_->position()) {
            c->current_position = c->cursor_->position();
            int pos = c->cursor_->position();
            const QRect qRect = editor->cursorRect(*c->cursor_);
            c->vertical_cursor->move(qRect.left(), qRect.top());
            c->vertical_cursor->show();
            int x_pos = qRect.center().x();
            int y_pos = qRect.topRight().y() - 4;
            c->horizontal_cursor->move(x_pos, y_pos);
            c->horizontal_cursor->show();
            format.setFontPointSize(14);
//        }
    }
}