
#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>
#include <QtGui/QTextCursor>
#include <QtWidgets/QLabel>
#include <cte/crdt/symbol.h>
#include <QtWidgets/QTextEdit>
#include <cte/protocol/document.h>
#include <cte/protocol/profile.h>

using namespace cte;
class UserInfo {

    QString username_, name_, surname_, email_;
    QImage icon_;
public:
    UserInfo();


    QString username() const;
    QString name() const;
    QString surname() const;
    QString email() const;
    QImage icon() const;
    bool selected;

    QTextCharFormat format;
    //cursor
    struct cursor{
        QLabel *vertical_cursor;
        QLabel *horizontal_cursor;
        QTextCursor *cursor_;
        int current_position;
    };
    void setIcon(const QImage &icon);
    QMap<QString,cte::Document> filename_to_owner_map;
    QHash<int,cursor*> site_id_to_cursor;


    void draw_background_char(QTextEdit *editor, int start, int end);

    UserInfo(Profile profile, QColor color);

    UserInfo(Profile profile);
    UserInfo& operator=(const Profile& other);
    const QColor color() const;

    void add_cursor(QTextEdit *editor, int newCursorPosition, int id);

    void change_cursor_position(QTextEdit *editor, int new_position, int id);

    QColor color_;

    bool has_cursor(int id) const;
    void remove_cursors(int id);
    void update_cursor(QTextEdit *editor);
    void clear_fields();
};

