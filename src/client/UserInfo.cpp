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
    c->cursor_label = new QLabel(editor);
    c->cursor_ = new QTextCursor(editor->document());
    new_position = qMin( new_position, editor->toPlainText().size());
    c->cursor_->setPosition(new_position);
    c->current_position =  new_position;
    QPixmap pix(2, 24);
    pix.fill(color_);
    c->cursor_label->setPixmap(pix);
    const QRect qRect = editor->cursorRect(*c->cursor_);
    c->cursor_label->move(qRect.left(), qRect.top());
    c->cursor_label->show();
    c->vertical_cursor = new QLabel(editor);
    int x_pos = qRect.center().x();
    int y_pos = qRect.topRight().y() - 4;
    QFontMetrics fm(editor->font());
    QRect bounds = fm.boundingRect(username_);
    c->vertical_cursor->move(x_pos,y_pos);
    c->vertical_cursor->setAutoFillBackground(true);
    c->vertical_cursor->setPalette(color_);
    c->vertical_cursor->setText(username_);
    site_id_to_cursor.insert(id,c);
    c->vertical_cursor->show();

}


void UserInfo::change_cursor_position(QTextEdit *editor, int new_position, int id) {
    auto c = site_id_to_cursor.find(id).value();
    new_position = qMin(new_position, editor->toPlainText().size());
    if(new_position != c->current_position) {
        c->cursor_->setPosition(new_position);
        c->current_position = new_position;
        const QRect qRect = editor->cursorRect(*c->cursor_);
        QLabel *cursor_label_ = site_id_to_cursor.find(id).value()->cursor_label;
        cursor_label_->move(qRect.left(), qRect.top());
        cursor_label_->show();
        auto *vertical_label = site_id_to_cursor.find(id).value()->vertical_cursor;
        int x_pos = qRect.center().x();
        int y_pos = qRect.topRight().y() - 4;
        QFontMetrics fm(editor->font());
        QRect bounds = fm.boundingRect(username_);
        vertical_label->move(x_pos, y_pos);
//        vertical_label->setAutoFillBackground(true);
//        vertical_label->setPalette(color_);
//        vertical_label->setText(username_);
        vertical_label->show();

    }

}

void UserInfo::remove_cursors(int id){
    QLabel* horizontal_cursor= site_id_to_cursor.find(id).value()->cursor_label;
    QLabel* vertical_cursor= site_id_to_cursor.find(id).value()->vertical_cursor;
    horizontal_cursor->hide();
    vertical_cursor->hide();
    delete horizontal_cursor;
    delete vertical_cursor;
    site_id_to_cursor.remove(id);
}

void UserInfo::update_cursor(QTextEdit *editor){

    for(auto c : site_id_to_cursor){
        if(c->current_position != c->cursor_->position()) {
            c->current_position = c->cursor_->position();
            int pos = c->cursor_->position();
            const QRect qRect = editor->cursorRect(*c->cursor_);
            c->cursor_label->move(qRect.left(), qRect.top());
            c->cursor_label->show();
            int x_pos = qRect.center().x();
            int y_pos = qRect.topRight().y() - 4;
            c->vertical_cursor->move(x_pos, y_pos);
            c->vertical_cursor->show();
        }
    }
}