/*
 * Author: Antonino Musmeci, Claudio Nuzzo
 */



#pragma once

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidgetItem>
#include <cte/client/newFileDialog.h>
#include <cte/client/myClient.h>
#include <cte/client/ProfileUpdateDialog.h>
#include <cte/client/OpenFileUrl.h>

namespace Ui {
    class loginTextEditor;

}

enum class OpenFileSource : int {
    create = 1,
    open_from_link = 2,
    open_from_list = 3
};

class loginTextEditor: public QStackedWidget {
Q_OBJECT

public:
    explicit loginTextEditor(QWidget *parent = nullptr);

private:
    QSharedPointer<myClient> client;
    Ui::loginTextEditor *ui;
    QSharedPointer<NewFileDialog> file_dialog;
    QSharedPointer<OpenFileUrl> open_dialog_;
    QSharedPointer<ProfileUpdateDialog> change_profile_dialog;
    OpenFileSource file_open_request_;
    QSharedPointer<texteditor>  editor;
    void cleanAll();

public slots:
    void open_profile_editor();

private slots:
    void on_connect_pushButton_clicked();
    void on_login_signup_pushButton_clicked();
    void on_signup_login_pushButton_clicked();
    void on_login_signin_pushButton_clicked();
    void on_singup_register_pushButton_clicked();
    void on_user_create_file_pushButton_clicked();
    void on_user_add_pushButton_clicked();
    void on_user_edit_profile_pushButton_clicked();
    void on_user_all_documents_pushButton_clicked();
    void on_user_own_documents_pushButton_clicked();
    void on_user_shared_documents_pushButton_clicked();
    void on_user_logout_pushButton_clicked();
    void on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item);
    //void on_user_share_pushButton_clicked();
    void share_file(const QString& filename);
    void connection_to_server(bool connected);
    void account_log_result(bool logged, const QString& error_message = QString(""));
    void handle_generic_error(const QString& error);
    void handle_timeout(const QString& type_request);
    void init_user_page();
    void display_documents(const QSet<Document>& documents);
    void open_editor(fileInfo file);
};




