/*
 * Author: Antonino Musmeci
 */



#pragma once

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidgetItem>
#include <cte/client/NewFileDialog.h>
#include <cte/client/myClient.h>
#include <cte/client/changePasswordDialog.h>
#include <cte/client/changeUsernameDialog.h>

namespace Ui {
    class loginTextEditor;

}
class loginTextEditor: public QStackedWidget {
Q_OBJECT

public:
    explicit loginTextEditor(QWidget *parent = nullptr);

private:
    QSharedPointer<myClient> client;
    Ui::loginTextEditor *ui;
    QSharedPointer<NewFileDialog> file_dialog;
    QSharedPointer<changeUsernameDialog> changeuser_dialog;
    QSharedPointer<changePasswordDialog> changepass_dialog;
    QSharedPointer<texteditor>  editor;
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
//    void on_user_share_pushButton_clicked();
    void share_file(const QString& filename);
    void account_log_result(bool logged, const QString& error_message = QString(""));
    void handle_generic_error(const QString& error);
    void init_user_page();
    void display_documents(const QSet<Document>& documents);
    void open_editor(fileInfo file);
};




