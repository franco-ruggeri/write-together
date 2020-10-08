/*
 * Author: Antonino Musmeci, Stefano Di Blasio
 */

#pragma once

#define APPLICATION "TEXTEDITOR"

#include <qtextedit.h>
#include <QtCore/QPointer>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
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
    void show_user_page(const QSet<Document> &docs);
    void show_profile_update();
    void share_file(const QString string);
//    void number_user_change();
private slots:

    void show_user_details();
    void file_to_pdf();
    void file_share();
    void file_close(bool spontaneous);
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

#ifndef QT_NO_CLIPBOARD
    void clipboard_changed();
#endif

private:

    QSharedPointer<QDockWidget>peers;
    QSharedPointer<QSplitter> peers_wrapper_;
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
    int number_online_users;
    int number_offline_users;
    QSharedPointer<QListWidget> active_user_list_;
    QSharedPointer<QListWidget> inactive_user_list_;
    QHash<QString,UserInfo> username_to_user;
#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif

    void setupPeersPanel();
    void draw_cursors();

//    void update_number_user();
};



