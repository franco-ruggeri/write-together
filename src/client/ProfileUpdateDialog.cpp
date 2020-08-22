/*
 * Author: Antonino Musmeci, Claudio Nuzzo
 */

#include <QStackedWidget>
#include <cte/client/myClient.h>
#include <QMessageBox>
#include <cte/client/ProfileUpdateDialog.h>
#include "ui_ProfileUpdateDialog.h"
#include <QTStylesheet.h>

ProfileUpdateDialog::ProfileUpdateDialog(QWidget *parent, QSharedPointer<myClient> client):
        QDialog(parent), ui(new Ui::ProfileUpdateDialog), client(client){
    ui->setupUi(this);

    //set password mode for line edits
    ui->changepass_confirmpass_lineEdit->setEchoMode(QLineEdit::Password);
    ui->changepass_newpass_lineEdit->setEchoMode(QLineEdit::Password);

    //button stylesheet
    ui->change_icon_pushButton->setStyleSheet(btnStylesheet);
    ui->changepass_change_pushButton->setStyleSheet(btnStylesheet);
    ui->changepass_cancel_pushButton->setStyleSheet(btnCancelStylesheet);

    //set username label
    ui->changeuser_username_label->setText(QString(client->user.username()));

    //set clearable fields
    ui->changeuser_newuser_lineEdit->setClearButtonEnabled(true);
    ui->changepass_newpass_lineEdit->setClearButtonEnabled(true);
    ui->changepass_confirmpass_lineEdit->setClearButtonEnabled(true);
    ui->changeuser_newname_lineEdit->setClearButtonEnabled(true);
    ui->changeuser_newsurname_lineEdit->setClearButtonEnabled(true);
    ui->changeuser_newemail_lineEdit->setClearButtonEnabled(true);

    // pre-fill fields
    ui->changeuser_newname_lineEdit->setText(client->user.name());
    ui->changeuser_newsurname_lineEdit->setText(client->user.surname());
    ui->changeuser_newemail_lineEdit->setText(client->user.email());

    //Icon QLineEdit
    QIcon user_icon(":/images/user.png");
    QIcon pass_icon(":/images/password.png");

    //Login
    ui->changeuser_newuser_lineEdit->addAction(user_icon, QLineEdit::LeadingPosition);
    ui->changepass_newpass_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);
    ui->changepass_confirmpass_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);

    connect(client.get(), &myClient::profile_update_result, this, &ProfileUpdateDialog::update_profile_result);
}

void ProfileUpdateDialog::on_changepass_change_pushButton_clicked() {
    qDebug("pressed");
    QString new_username = ui->changeuser_newuser_lineEdit->text();
    QString new_name = ui->changeuser_newname_lineEdit->text();
    QString new_surname = ui->changeuser_newsurname_lineEdit->text();
    QString new_email = ui->changeuser_newemail_lineEdit->text();
    QImage new_icon; // to be added
    QString new_password = ui->changepass_newpass_lineEdit->text();
    QString new_passowrd_confirm = ui->changepass_confirmpass_lineEdit->text();
    if (new_username.isEmpty()) new_username = client->user.username();
    bool with_password = true;
    QString min_req = tr("\nMinimum requirements:\n") + tr(" - 8+ characters long\n") + tr(" - at least one uppercase letter\n")
            + tr(" - at least one lowercase letter\n") + tr(" - at least one number\n");

    if (new_password.isEmpty() && new_passowrd_confirm.isEmpty()) {
        with_password = false;
    }
    if (with_password) {
        QString error_display;
        if (!utility::check_password_validity(new_password) || !utility::check_password_validity(new_passowrd_confirm)) {
            if (new_password != new_passowrd_confirm) {
                error_display += tr("Please check to have inserted the same password.\n");
            }
            error_display += tr("Please remember to satisfy the minimum requirements for the new password you set.\n");
            QMessageBox::warning(this, tr("Input check"), error_display + min_req);
            itsOkToClose = false;
            return;
        }
        if (new_password != new_passowrd_confirm) {
            error_display += tr("Please check to have inserted the same password.\n");
            QMessageBox::warning(this, tr("Input check"), error_display);
            itsOkToClose = false;
            return;
        }
    }
    client->update_profile(new_username, new_email, new_name, new_surname, new_icon, new_password);
}

void ProfileUpdateDialog::update_profile_result(bool result, const QString &error_message) {
    if (result) {
        ui->changeuser_username_label->setText(client->user.username());
        QMessageBox::information(this, tr("Update successful"), tr("Your data have been correctly updated"));
    } else {
        QMessageBox::warning(this, tr("Update problem"), tr("The server reported the following error.\n") + error_message);
    }
    ui->changeuser_newuser_lineEdit->clear();
    itsOkToClose = true;
    emit accept();
}

void ProfileUpdateDialog::on_changepass_cancel_pushButton_clicked(){
    emit reject();
}

#include <iostream>
void ProfileUpdateDialog::accept(){
    if (itsOkToClose){
        QDialog::accept();
    }
}

