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
    void share_file(const QString string);

private slots:

    void file_to_pdf();
    void file_share();
    void file_close();

    void setupEditActions();
    void setupUserActions();
    void setupPeers();
    void readyRead();
    void contentsChange(int position, int charsRemoved, int charsAdded);
    void cursorPositionChanged();
    void textChange();

private:
    QSharedPointer<QDockWidget>connected_client ;
    QSharedPointer<QTextEdit> editor;
    QSharedPointer<myClient> client;
    QSharedPointer<SharedEditor> shared_editor;
    QAction *actionUndo;
    QAction * actionRedo;
    QAction *show_peers;
    fileInfo file;
    QSharedPointer<QListWidget>list_user;
    QMap<QString, UserInfo> map_username_to_User;
//    QHash<QString,Symbol> connected_user;
    QHash<int,UserInfo> site_id_to_user;
#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif




};



