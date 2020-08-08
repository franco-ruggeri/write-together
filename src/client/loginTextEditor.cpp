/*
 * Author: Antonino Musmeci
 */

#include <QMessageBox>
#include <QPixmap>
#include <cte/client/changeUsernameDialog.h>
#include <cte/client/loginTextEditor.h>
#include "ui_loginTextEditor.h"
#include <cte/client/utility.h>
#include <cte/client/fileInfo.h>
#include <QClipboard>
#include <cte/protocol/Document.h>

loginTextEditor::loginTextEditor(QWidget *parent) : QStackedWidget(parent), ui(new Ui::loginTextEditor) {
    file_dialog = nullptr;
    ui->setupUi(this);
    this->setCurrentIndex(3); // open widget 3 (connect page)
    this->show();
    client = QSharedPointer<myClient>::create();
    ui->signup_password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->login_password_lineEdit->setEchoMode(QLineEdit::Password);
   
}

/**************home-page function***************/
void loginTextEditor::on_connect_pushButton_clicked() {
    QString ip_address = ui->connect_address_lineEdit_->text();
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

    QString username = ui->login_email_lineEdit->text();
    QString password = ui->login_password_lineEdit->text();
    if(username.isEmpty() || password.isEmpty())
        return;
    /*** to do login function **/
    std::tuple valid = client->login(username,password);
    if(std::get<0>(valid)) {
        init_user_page();
    }
    else
        QMessageBox::warning(this, "WARNING",
                             std::get<1>(valid));
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

    QString name = ui->signup_name_lineEdit->text();
    QString surname = ui->signup_surname_lineEdit->text();

    std::tuple valid = client->signup(username, email,password , name, surname);
    if(std::get<0>(valid)) {
       init_user_page();
    }
    else
        QMessageBox::warning(this, "WARNING",
                             std::get<1>(valid));

}

/********** user page function *******************/

void loginTextEditor::init_user_page() {
    QSet<Document> documents = client->get_documents_form_server();
    ui->user_file_listWidget->clear();
    QStringList file_list;
    for(const auto& d : documents) {
        file_list.push_back(d.full_name());
//        client->user.filename_to_owner_map.insert(d.full_name(),d);
    }
    ui->user_file_listWidget->addItems(file_list);
    ui->user_file_listWidget->setCurrentRow( 0 );
    this->setCurrentIndex(0); // 0 -> user page
}
void loginTextEditor::on_user_create_file_pushButton_clicked() {
    if(file_dialog == nullptr) {
        file_dialog = QSharedPointer<newFileDialog>::create(this, client, editor);
        connect(file_dialog.get(), &newFileDialog::open_editor, this, &loginTextEditor::open_editor);
    }
    file_dialog->setModal(true);
    file_dialog->show();
}

void loginTextEditor::on_user_logout_pushButton_clicked() {
    client->logout();
    cleanAll();
    this->setCurrentIndex(3);
}

void loginTextEditor::on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    fileInfo file =  client->open_file(item->text());
    open_editor(file);
}

void loginTextEditor::on_user_change_password_pushButton_clicked() {

    if(changepass_dialog == nullptr)
        changepass_dialog = QSharedPointer<changePasswordDialog>::create(this,this->client);
    changepass_dialog->setModal(true);
    changepass_dialog->show();
}

void loginTextEditor::on_user_change_username_pushButton_clicked() {
    if(changeuser_dialog == nullptr)
        changeuser_dialog = QSharedPointer<changeUsernameDialog>::create(this,this->client);
    changeuser_dialog->setModal(true);
    changeuser_dialog->show();
}


void loginTextEditor::open_editor(fileInfo file){

    this->hide();
    editor = QSharedPointer<texteditor>::create(nullptr,client,file);
    connect(editor.get(), &texteditor::show_user_page, this, &loginTextEditor::show);
    connect(editor.get(), &texteditor::share_file, this, &loginTextEditor::share_file);
    editor->show();
    editor->init_cursors();

}

void loginTextEditor::cleanAll(){
    ui->signup_password_lineEdit->clear();
    ui->signup_email_lineEdit->clear();
    ui->signup_username_lineEdit->clear();
    ui->signup_name_lineEdit->clear();
    ui->signup_username_lineEdit->clear();
    ui->login_password_lineEdit->clear();
    ui->login_email_lineEdit->clear();
    ui->user_file_listWidget->clear();
}

void loginTextEditor::share_file(const QString& shared_link){
//    std::optional<QString> response = client->get_uri(filename);
    QMessageBox msgBox;
    QClipboard *cb = QApplication::clipboard();
    msgBox.setText(" URI: " + shared_link);
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Copy"), QMessageBox::YesRole);
    msgBox.addButton(tr("Close"), QMessageBox::NoRole);
    msgBox.exec();
    if (msgBox.clickedButton()==pButtonYes) {
        cb->setText(shared_link);
    }
}
