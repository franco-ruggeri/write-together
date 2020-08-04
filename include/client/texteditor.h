/*
 * Author: Antonino Musmeci
 */

#pragma once
#define APPLICATION "TEXTEDITOR"
#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QPointer>
#include <qtextedit.h>
#include <QListWidget>
#include "crdt/SharedEditor.h"
#include "myClient.h"
#include "fileInfo.h"
using namespace editor;
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
    QMap<QString, Profile> map_username_to_User;
    QHash<QString,Symbol> connected_user;

#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif

};



