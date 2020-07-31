/*
 * Author: Antonino Musmeci
 */



#pragma once
#include <QStackedWidget>
#include <QListWidgetItem>
#include "newFileDialog.h"
#include "myClient.h"
#include "changePasswordDialog.h"
#include "changeUsernameDialog.h"

namespace Ui {
    class loginTextEditor;

}
class loginTextEditor: public QStackedWidget {
Q_OBJECT

public:
    explicit loginTextEditor(QWidget *parent = nullptr);
    void open_editor(const QString& filename,bool newfile);

private:
    QSharedPointer<myClient> client;
    Ui::loginTextEditor *ui;
    QSharedPointer< newFileDialog> file_dialog;
    QSharedPointer<changeUsernameDialog> changeuser_dialog;
    QSharedPointer<changePasswordDialog> changepass_dialog;
    QSharedPointer<texteditor>  editor;
    void init_user_page(std::vector<QString>);
    void cleanAll();

private slots:
    void on_connect_pushButton_clicked();
    void on_login_signup_pushButton_clicked();
    void on_signup_login_pushButton_clicked();
    void on_login_signin_pushButton_clicked();
    void on_singup_register_pushButton_clicked();
    void on_user_create_file_pushButton_clicked();
    void on_user_change_password_pushButton_clicked();
    void on_user_logout_pushButton_clicked();
    void on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_user_change_username_pushButton_clicked();
    void on_user_share_pushButton_clicked();
    void share_file(QString filename);

    void init_user_page();
};




