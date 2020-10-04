/*
 * Author: Antonino Musmeci, Stefano Di Blasio
 */

#include <QMessageBox>
#include <QPixmap>
#include <QtCore/QDebug>
#include <cte/client/loginTextEditor.h>
#include "ui_loginTextEditor.h"
#include <cte/client/fileInfo.h>
#include <QClipboard>
#include <cte/protocol/Document.h>
#include <cte/network/network_utility.h>
#include <QTStylesheet.h>


loginTextEditor::loginTextEditor(QWidget *parent) : QStackedWidget(parent), ui(new Ui::loginTextEditor) {
    file_dialog = nullptr;
    ui->setupUi(this);
    this->setCurrentIndex(4); // open widget 3 (connect page)
    this->show();
    client = QSharedPointer<myClient>::create();
    ui->signup_password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->login_password_lineEdit->setEchoMode(QLineEdit::Password);
    connect(client.data(), &myClient::generic_error, this, &loginTextEditor::handle_generic_error);
    connect(client.data(), &myClient::timeout_expired, this, &loginTextEditor::handle_timeout);
    connect(client.data(), &myClient::authentication_result, this, &loginTextEditor::account_log_result);
    connect(client.data(), &myClient::user_documents, this, &loginTextEditor::display_documents);
    connect(client.data(), &myClient::document, this, &loginTextEditor::open_editor);
    connect(client.data(), &myClient::host_connected, this, &loginTextEditor::connection_to_server);
    connect(client.data(), &myClient::server_disconnected, this, &loginTextEditor::handle_disconnection_server);
    QMetaObject::invokeMethod(client.data(), "connect", Qt::QueuedConnection);

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

void loginTextEditor::handle_timeout(const QString &message_request) {
    QMessageBox timeout_box; // evaluate if store it as a member property, to dismiss it if response arrives
    timeout_box.setWindowTitle(tr("Attention"));
    timeout_box.setText(tr("The processing of your ") + message_request + tr(" request is taking more than expected on the server.\nPlease wait it"));
    auto dismiss_button = timeout_box.addButton(QMessageBox::Ok); // this may be useful for "remote" dismissing
    timeout_box.setDefaultButton(dismiss_button);
    timeout_box.setEscapeButton(dismiss_button);
    timeout_box.exec();
}

void loginTextEditor::account_log_result(bool logged, const QString &error_message) {
    if (logged) {
        init_user_page();
    } else {
        QMessageBox::warning(this, tr("Attention"), error_message);
    }
    return;
}

void loginTextEditor::connection_to_server(bool connected) {
    if (connected) {
        this->setCurrentIndex(2);
        //this->setCurrentIndex(3);
    } else {
        if (this->currentIndex() == 3) {
            QMessageBox::warning(this, tr("Server unreachable"), tr("The server is not responding now. Please try later."));
        }
        this->setCurrentIndex(3);
    }
}

void loginTextEditor::handle_disconnection_server() {
    // nothing should be done if already in connecting page
    if (this->currentIndex() != 3 && this->currentIndex() != 4) {
        this->clear_user_fields();
        client->destroy_previous_connection();
        if (editor && editor->isVisible()) {
            client->connect_response();
            editor->close();
        }
        if (file_dialog && file_dialog->isVisible()) {
            file_dialog->close();
        }
        if (open_dialog_ && open_dialog_->isVisible()) {
            open_dialog_->close();
        }
        if (change_profile_dialog && change_profile_dialog->isVisible()) {
            change_profile_dialog->close();
        }
        this->setCurrentIndex(4);
        QMetaObject::invokeMethod(client.data(), "connect", Qt::QueuedConnection);
    }
}

/**************home-page function***************/
void loginTextEditor::on_connect_pushButton_clicked() {
    QString ip_address = ui->connect_address_lineEdit_->text();
    if (!cte::check_ip_address(ip_address)) {
        QMessageBox::warning(this, "WARNING", "Insert a valid IP address");
        return;
    }
    client->connect(ip_address); // TODO: add the port
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
    if (!cte::Profile::check_email(email.trimmed())) {
        QMessageBox::warning(this, "WARNING", "Please insert a valid email");
        return;
    }
    QString username = ui->signup_username_lineEdit->text();
    if (!cte::Profile::check_username(username.trimmed())) {

        QMessageBox::warning(this, "WARNING", "Username should be at least 3 characters long");
        return;
    }
    QString password = ui->signup_password_lineEdit->text();
    if (!cte::Profile::check_password(password)) {
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
    if (change_profile_dialog == nullptr) {
        change_profile_dialog = QSharedPointer<ProfileUpdateDialog>::create(this, this->client);
        change_profile_dialog->setWindowModality(Qt::WindowModal);
    }
    if (open_dialog_ == nullptr) {
        open_dialog_ = QSharedPointer<OpenFileUrl>::create(this, this->client);
        open_dialog_->setModal(true);
    }
    this->setCurrentIndex(0); // 0 -> user page
    client->get_documents_form_server();
}

void loginTextEditor::display_documents(const QSet<Document> &documents) {
    // just a try for ProfileUpdateDialog modality
    if (change_profile_dialog && change_profile_dialog->isVisible()) {
        change_profile_dialog->hide();
        change_profile_dialog->show();
    }
    QStringList file_list;
    ui->user_file_listWidget->clear();
    ui->label_list_items->setStyleSheet(list_items_title);
    if (documents.empty()){
        ui->label_list_items->setText(QString("No documents available."));
    }
    else {
        ui->label_list_items->setText(QString("Documents available."));
        for (const auto& d: documents) {
            file_list.push_back(d.full_name());
            client->user.filename_to_owner_map.insert(d.full_name(), d);
        }
        ui->user_file_listWidget->addItems(file_list);
        ui->user_file_listWidget->setCurrentRow(0);
    }
}

void loginTextEditor::on_user_create_file_pushButton_clicked() {
    file_open_request_ = OpenFileSource::create;
    if(file_dialog == nullptr) {
        file_dialog = QSharedPointer<NewFileDialog>::create(this, client, editor);

    }
    file_dialog->setModal(true);
    file_dialog->show();
}

void loginTextEditor::on_user_add_pushButton_clicked() {
    file_open_request_ = OpenFileSource::open_from_link;
    open_dialog_->show();
}

void loginTextEditor::on_user_logout_pushButton_clicked() {
    client->logout();
    clear_user_fields();
    this->setCurrentIndex(3);
}

void loginTextEditor::on_user_file_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    file_open_request_ = OpenFileSource::open_from_list;
    client->open_file(item->text());
}

void loginTextEditor::open_profile_editor() {
    change_profile_dialog->show();
}

void loginTextEditor::on_user_edit_profile_pushButton_clicked(){
    change_profile_dialog->show();
}

void loginTextEditor::on_user_all_documents_pushButton_clicked(){
    QStringList file_list;
    ui->user_all_documents_pushButton->setStyleSheet(btnSelectedFilterStylesheet);
    ui->user_own_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_shared_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_file_listWidget->clear();
    QList documents = client->user.filename_to_owner_map.values();
    for (auto d:documents){
        file_list.push_back(d.full_name());
    }
    if (file_list.length() == 0){
        ui->label_list_items->setText(QString("No documents available."));
    }
    else {
        ui->label_list_items->setText(QString("Documents available."));
        ui->user_file_listWidget->addItems(file_list);
        ui->user_file_listWidget->setCurrentRow(0);
    }
}

void loginTextEditor::on_user_own_documents_pushButton_clicked(){
    QStringList file_list;

    ui->user_all_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_own_documents_pushButton->setStyleSheet(btnSelectedFilterStylesheet);
    ui->user_shared_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);

    ui->user_file_listWidget->clear();
    QList documents = client->user.filename_to_owner_map.values();
    for (auto d:documents){
        if(d.owner() == client->user.username()) {
            file_list.push_back(d.full_name());
        }
    }
    if (file_list.length() == 0){
        ui->label_list_items->setText(QString("You don't own any document."));
    }
    else {
        ui->label_list_items->setText(QString("Documents you own."));
        ui->user_file_listWidget->addItems(file_list);
        ui->user_file_listWidget->setCurrentRow(0);
    }
}

void loginTextEditor::on_user_shared_documents_pushButton_clicked(){
    ui->user_all_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_own_documents_pushButton->setStyleSheet(btnFilterDocumentsStylesheet);
    ui->user_shared_documents_pushButton->setStyleSheet(btnSelectedFilterStylesheet);
    QStringList file_list;
    ui->user_file_listWidget->clear();
    QList documents = client->user.filename_to_owner_map.values();
    for (auto d:documents){
        if(d.owner() != client->user.username()) {
            file_list.push_back(d.full_name());
        }
    }
    if (file_list.length() == 0){
        ui->label_list_items->setText(QString("No documents have been shared with you."));
    }
    else {
        ui->label_list_items->setText(QString("Documents shared with you."));
        ui->user_file_listWidget->addItems(file_list);
        ui->user_file_listWidget->setCurrentRow(0);
    }

}

void loginTextEditor::open_editor(fileInfo file){
    if (file_dialog != nullptr && file_dialog->isVisible())
        file_dialog->hide();
    if (file_open_request_ == OpenFileSource::open_from_link) {
        open_dialog_->clear_content();
    }
    this->hide();
    editor = QSharedPointer<texteditor>::create(nullptr,client,file);
    connect(editor.data(), &texteditor::show_user_page, this, &QWidget::show);
    connect(editor.data(), &texteditor::show_user_page, this, &loginTextEditor::display_documents);
    connect(editor.data(), &texteditor::share_file, this, &loginTextEditor::share_file);
    connect(editor.data(), &texteditor::show_profile_update, this, &loginTextEditor::open_profile_editor);
    editor->show();
    editor->init_cursors();
}

void loginTextEditor::clear_user_fields(){
    ui->signup_password_lineEdit->clear();
    ui->signup_email_lineEdit->clear();
    ui->signup_username_lineEdit->clear();
    ui->signup_name_lineEdit->clear();
    ui->signup_surname_lineEdit->clear();
    ui->login_password_lineEdit->clear();
    ui->login_email_lineEdit->clear();
    ui->label_username_user_page->clear();
    ui->user_file_listWidget->clear();
    if (open_dialog_) open_dialog_->clear_content();
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
