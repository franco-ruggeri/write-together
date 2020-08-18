/*
 * Author: Antonino Musmeci
 */

#include <QMessageBox>
#include <QPixmap>
#include <QtCore/QDebug>
#include <cte/client/changeUsernameDialog.h>
#include <cte/client/loginTextEditor.h>
#include "ui_loginTextEditor.h"
#include <cte/client/utility.h>
#include <cte/client/fileInfo.h>
#include <QClipboard>
#include <cte/protocol/Document.h>
#include <QTStylesheet.h>


loginTextEditor::loginTextEditor(QWidget *parent) : QStackedWidget(parent), ui(new Ui::loginTextEditor) {
    file_dialog = nullptr;
    ui->setupUi(this);
    this->setCurrentIndex(3); // open widget 3 (connect page)
    this->show();
    client = QSharedPointer<myClient>::create();
    ui->signup_password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->login_password_lineEdit->setEchoMode(QLineEdit::Password);
    connect(client.get(), &myClient::generic_error, this, &loginTextEditor::handle_generic_error);
    connect(client.get(), &myClient::authentication_result, this, &loginTextEditor::account_log_result);
    connect(client.get(), &myClient::user_documents, this, &loginTextEditor::display_documents);
    connect(client.get(), &myClient::document, this, &loginTextEditor::open_editor);

    //Clear button enabled
    ui->login_email_lineEdit->setClearButtonEnabled(true);
    ui->login_password_lineEdit->setClearButtonEnabled(true);
    ui->signup_name_lineEdit->setClearButtonEnabled(true);
    ui->signup_surname_lineEdit->setClearButtonEnabled(true);
    ui->signup_email_lineEdit->setClearButtonEnabled(true);
    ui->signup_username_lineEdit->setClearButtonEnabled(true);
    ui->signup_password_lineEdit->setClearButtonEnabled(true);

    //Image login
    QPixmap login_image(":/images/collaboration.png");
    ui->label_image_login->setPixmap(login_image);

    //Icon QLineEdit
    QIcon user_icon(":/images/user.png");
    QIcon pass_icon(":/images/password.png");
    QIcon mail_icon(":/images/mail.png");

    //Login
    ui->login_email_lineEdit->addAction(user_icon, QLineEdit::LeadingPosition);
    ui->login_password_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);

    //Signup
    ui->signup_email_lineEdit->addAction(mail_icon, QLineEdit::LeadingPosition);
    ui->signup_username_lineEdit->addAction(user_icon, QLineEdit::LeadingPosition);
    ui->signup_password_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);

    //buttons hover feedback
    //connect
    ui->connect_pushButton->setStyleSheet(btnStylesheet);
    //login
    ui->login_signup_pushButton->setStyleSheet(btnRedirectStylesheet);
    ui->login_signin_pushButton->setStyleSheet(btnStylesheet);
    //signup
    ui->singup_register_pushButton->setStyleSheet(btnStylesheet);
    ui->signup_login_pushButton->setStyleSheet(btnRedirectStylesheet);
    //user_page
    ui->user_edit_profile_pushButton->setStyleSheet(btnEditProfileStylesheet);
    ui->user_logout_pushButton->setStyleSheet(btnLogOutStylesheet);
    ui->user_add_pushButton->setStyleSheet(btnStylesheet);
    ui->user_create_file_pushButton->setStyleSheet(btnNewFileStylesheet);
    ui->user_add_pushButton->setStyleSheet(btnNewFileStylesheet);
    ui->user_all_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_shared_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_own_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);

}

void loginTextEditor::handle_generic_error(const QString &error) {
    QMessageBox::critical(this, tr("Fatal"), tr("This is a generic error report, mainly due to bug/unexpected control flow.\n\n") + error);
}

void loginTextEditor::account_log_result(bool logged, const QString &error_message) {
    if (logged) {
        init_user_page();
    } else {
        QMessageBox::critical(this, tr("Attention"), error_message);
    }
    return;
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

    client->login(username,password);
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

    client->signup(username, email,password , name, surname);
}

/********** user page function *******************/

void loginTextEditor::init_user_page() {
    ui->user_file_listWidget->clear();
    ui->user_file_listWidget->setStyleSheet(list_stylesheet);
    ui->label_username_user_page->setFont(QFont("Roboto Thin", 20, 1, true));
    ui->label_username_user_page->setText(QString(client->user.username()));
    this->setCurrentIndex(0); // 0 -> user page
    client->get_documents_form_server();
}

void loginTextEditor::display_documents(const QSet<Document> &documents) {
    QStringList file_list;
    if (documents.empty()){
        ui->label_list_items->setText(QString("No documents available."));
    }
    else {
        ui->label_list_items->setText(QString("Documents list."));
        for (const auto& d: documents) {
            file_list.push_back(d.full_name());
            client->user.filename_to_owner_map.insert(d.full_name(), d);
        }
        ui->user_file_listWidget->addItems(file_list);
        ui->user_file_listWidget->setCurrentRow(0);
    }
}

void loginTextEditor::on_user_create_file_pushButton_clicked() {
    if(file_dialog == nullptr) {
        file_dialog = QSharedPointer<NewFileDialog>::create(this, client, editor);

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
    client->open_file(item->text());
}

void loginTextEditor::on_user_edit_profile_pushButton_clicked(){
    if(changepass_dialog == nullptr)
        changepass_dialog = QSharedPointer<changePasswordDialog>::create(this,this->client);
    changepass_dialog->setModal(true);
    changepass_dialog->show();
}

/*
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
*/

void loginTextEditor::open_editor(fileInfo file){
    if (file_dialog != nullptr && file_dialog->isVisible())
        file_dialog->hide();
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