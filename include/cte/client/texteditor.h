/*
 * Author: Antonino Musmeci
 */

#pragma once

#define APPLICATION "TEXTEDITOR"

#include <qtextedit.h>
#include <QtCore/QPointer>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>
#include <cte/crdt/SharedEditor.h>
#include <cte/client/myClient.h>
#include <cte/client/fileInfo.h>

using namespace cte;
class texteditor : public QMainWindow{
    Q_OBJECT


public:
    texteditor(QStackedWidget *parent, QSharedPointer<myClient> client, fileInfo file);

    bool change_from_server;
    void setupFileActions();
    void closeEvent(QCloseEvent *event) override;
    void init_cursors();

signals:
    void show_user_page();
    void show_profile_update();
    void share_file(const QString string);

private slots:

    void show_user_details();
    void file_to_pdf();
    void file_share();
    void file_close();
    void setupEditActions();
    void setupUserActions();
    void setupPeers();
    void contentsChange(int position, int charsRemoved, int charsAdded);
    void cursorPositionChanged();
    void textChange();
    void on_list_user_itemClicked(QListWidgetItem *item);
    void remote_open(const Profile &profile, int site_id);
    void remote_close(const QString &username, int site_id);
    void remote_cursor(const Symbol &symbol, const QString &username);
    void remote_erase(const Symbol &symbol);
    void remote_insert(const Symbol& symbol);
    QColor generate_color();

private:

    QSharedPointer<QDockWidget>peers;
    QSharedPointer<QTextEdit> editor;
    QSharedPointer<myClient> client;
    QSharedPointer<SharedEditor> shared_editor;
    QAction *actionUndo;
    QAction * actionRedo;
    QAction *show_peers;
    QAction *peer_profile;
    fileInfo file;
    double h;
    int current_position;
    QSharedPointer<QListWidget>list_user;
    QHash<QString,UserInfo> username_to_user;
    QHash<QString,int> username_to_row;
    int user_row_;
#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif


    void draw_cursors();
};



