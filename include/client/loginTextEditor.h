/*
 * Author: Antonino Musmeci
 */



#pragma once
#include <QStackedWidget>
#include <QListWidgetItem>
#include "newFileDialog.h"
#include "myClient.h"

namespace Ui {
    class loginTextEditor;

}

class loginTextEditor: public QStackedWidget {
    Q_OBJECT
public:
    explicit loginTextEditor(QWidget *parent = nullptr);
    ~loginTextEditor();

private:
    std::shared_ptr<myClient> client;
    Ui::loginTextEditor *ui;
    newFileDialog * file_dialog;
    texteditor *editor;
    void init_user_page(std::vector<QString>);


private slots:
    void on_connect_pushButton_clicked();
    void on_login_signup_pushButton_clicked();
    void on_signup_login_pushButton_clicked();
    void on_login_signin_pushButton_clicked();
    void on_singup_register_pushButton_clicked();
    void on_user_create_file_pushButton_clicked();
    void on_user_logout_pushButton_clicked();
    void on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void open_editor(QString filename);
};




