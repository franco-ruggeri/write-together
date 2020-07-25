/*
 * Author: Antonino Musmeci
 */

#include <QMessageBox>
#include "client/loginTextEditor.h"
#include "ui_loginTextEditor.h"
#include "client/utility.h"
#include "client/fileInfo.h"
loginTextEditor::loginTextEditor(QWidget *parent) : QStackedWidget(parent), ui(new Ui::loginTextEditor) {
    file_dialog = nullptr;
    ui->setupUi(this);
    this->setCurrentIndex(3); // open widget 3 (connect page)
    this->show();
    client = std::make_shared<myClient>();
}

loginTextEditor::~loginTextEditor(){
    delete file_dialog;
}

/**************home-page function***************/
void loginTextEditor::on_connect_pushButton_clicked() {
    QString ip_address = ui->connect_address_lineEdit->text();
    if (!utility::check_ip_address(ip_address)) {
        QMessageBox::warning(this, "WARNING", "Insert a valid IP address");
        return;
    }
    bool connect = client->connect(ip_address);
    if(connect) this->setCurrentIndex(2);
    else QMessageBox::warning(this, "ERROR", "Unable To Reach the Server");
}

/**************login page function***************/
void loginTextEditor::on_login_signup_pushButton_clicked() {
    this->setCurrentIndex(1); //-> signUp page
}

void loginTextEditor::on_login_signin_pushButton_clicked() {
    QString email = ui->login_email_lineEdit->text();
    QString password = ui->login_password_lineEdit->text();
    /*** to do login function **/
    std::tuple valid = client->login(email,password);
    if(std::get<0>(valid))
        init_user_page(std::get<1>(valid));
    else
        QMessageBox::warning(this, "WARNING",
                             std::get<1>(valid)[0]);
}


/**************register page function***************/

void loginTextEditor::on_signup_login_pushButton_clicked() {
    this->setCurrentIndex(2); //2-> login page
}

void loginTextEditor::on_singup_register_pushButton_clicked() {
    QString email = ui->signup_email_lineEdit->text();
    if (!utility::check_email_validity(email.trimmed())) {
        QMessageBox::warning(this, "WARNING", "Please insert a valid email");
        return;
    }
    QString username = ui->signup_username_lineEdit->text();
    if (!utility::check_username_validity(username.trimmed())) {

        QMessageBox::warning(this, "WARNING", "Username should be at least 3 characters long");
        return;
    }
    QString password = ui->signup_password_lineEdit->text();
    if (!utility::check_password_validity(password)) {
        QMessageBox::warning(this, "WARNING",
                             "your password must be at least 8 characters long including at least one number and both lower and upper characters");
        return;
    }

    std::tuple valid = client->signup(username, email,password);
    if(std::get<0>(valid))
        init_user_page(std::get<1>(valid));
    else
        QMessageBox::warning(this, "WARNING",
                             std::get<1>(valid)[0]);

}


/********** user page function *******************/

void loginTextEditor::init_user_page(std::vector<QString> files) {
    ui->user_file_listWidget->clear();
    QStringList file_list;
    for(const auto& f : files)
        file_list.push_back(f);
    ui->user_file_listWidget->addItems(file_list);
    this->setCurrentIndex(0); // 0 -> user page

}

void loginTextEditor::on_user_create_file_pushButton_clicked() {
    if(file_dialog == nullptr)
        file_dialog = new newFileDialog(this,client,editor);
    file_dialog->setModal(true);
    file_dialog->show();
}

void loginTextEditor::on_user_logout_pushButton_clicked() {
    client->logout();
    cleanAll();
    this->setCurrentIndex(3);
}

void loginTextEditor::on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    open_editor(item->text());
}

void loginTextEditor::open_editor(QString filename){

    this->hide();
    std::tuple<bool,QString> result =  client->open_file(filename);
    if(std::get<0>(result)){
        auto file = fileInfo("name",std::get<1>(result));
        editor = new texteditor(nullptr,client,file);
        editor->show();
        //    connect(text_editor, &text_editor::showUserPage, this, &loginTextEditor::show);
    }
    else
        QMessageBox::warning(this, "WARNING",
                             std::get<1>(result));

}


void loginTextEditor::cleanAll(){
    ui->signup_password_lineEdit->clear();
    ui->signup_email_lineEdit->clear();
    ui->signup_username_lineEdit->clear();
    ui->login_password_lineEdit->clear();
    ui->login_email_lineEdit->clear();
    ui->user_file_listWidget->clear();
}