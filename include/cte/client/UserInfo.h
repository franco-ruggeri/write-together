
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


//cursor
    QTextCursor *cursor_;
    QLabel* cursor_label_;
    Symbol cursor_position_;

public:
    UserInfo();
    UserInfo(Profile profile);
//    Profile(const QString& username, const QString& name, const QString& surname, const QImage& icon);
//    Profile(const QJsonObject& json_object);

//    bool operator==(const Profile& other) const;

    QString username() const;
    QString name() const;
    QString surname() const;
    QImage icon() const;


    //cursors

    void setIcon(const QImage &icon);
    QMap<QString,cte::Document> filename_to_owner_map;
    void init_cursor(QTextEdit *editor, int newCursorPosition);
    void change_cursor_position(QTextEdit *editor, int new_position);
};

