
#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>
#include <QtGui/QTextCursor>
#include <QtWidgets/QLabel>
#include <cte/crdt/Symbol.h>
#include <QtWidgets/QTextEdit>
#include <cte/protocol/Document.h>
#include <cte/protocol/Profile.h>

using namespace cte;
class UserInfo {

    QString username_, name_, surname_;
    QImage icon_;

    Symbol cursor_position_;


public:
    UserInfo();


    QString username() const;
    QString name() const;
    QString surname() const;
    QImage icon() const;
    bool selected;

    //cursor
    struct cursor{
        QLabel *cursor_label;
        QLabel *vertical_cursor;
        QTextCursor *cursor_;
        int current_position;
    };
    void setIcon(const QImage &icon);
    QMap<QString,cte::Document> filename_to_owner_map;
    QHash<int,cursor*> site_id_to_cursor;


    void draw_background_char(QTextEdit *editor, int start, int end);

    UserInfo(Profile profile, QColor color);

    UserInfo(Profile profile);

    const QColor color();

    void add_cursor(QTextEdit *editor, int newCursorPosition, int id);

    void change_cursor_position(QTextEdit *editor, int new_position, int id);

    QColor color_;

    void remove_cursors(int id);


    void update_cursor(QTextEdit *editor);
};

