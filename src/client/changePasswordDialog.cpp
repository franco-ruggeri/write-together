/*
 * Author: Antonino Musmeci, Claudio Nuzzo
 */

#include <QStackedWidget>
#include <cte/client/myClient.h>
#include <QMessageBox>
#include <cte/client/changePasswordDialog.h>
#include "ui_changePasswordDialog.h"
#include <QTStylesheet.h>

changePasswordDialog::changePasswordDialog(QWidget *parent, QSharedPointer<myClient> client):
        QDialog(parent),ui(new Ui::changePasswordDialog), client(client){
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

    //Icon QLineEdit
    QIcon user_icon(":/images/user.png");
    QIcon pass_icon(":/images/password.png");

    //Login
    ui->changeuser_newuser_lineEdit->addAction(user_icon, QLineEdit::LeadingPosition);
    ui->changepass_newpass_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);
    ui->changepass_confirmpass_lineEdit->addAction(pass_icon, QLineEdit::LeadingPosition);


}

void changePasswordDialog::on_changepass_change_pushButton_clicked() {
    qDebug("pressed");
    QString new_password = ui->changepass_newpass_lineEdit->text();
    QString confirmed_new_password = ui->changepass_confirmpass_lineEdit->text();

    if (!utility::check_password_validity(new_password) || !utility::check_password_validity(confirmed_new_password)) {
        QMessageBox::warning(this, "WARNING",
                             "your password must be at least 8 characters long including at least one number and both lower and upper characters");

        itsOkToClose = false;
    }
    else {

        if (new_password == confirmed_new_password) {
            bool response = client->change_password(new_password);
            if (response) {
                QMessageBox::information(this, "PASSWORD CHANGED", "Your password has been changed");
                itsOkToClose = true;
            }
            else {
                QMessageBox::warning(this, "WARNING", "ERROR");
                itsOkToClose = true;
            }

        } else {
            QMessageBox::warning(this, "WARNING", "The password must be equal");
            itsOkToClose = false;
        }
    }
    emit accept();
}
void changePasswordDialog::on_changepass_cancel_pushButton_clicked(){
    emit reject();
}

#include <iostream>
void changePasswordDialog::accept(){
    if (itsOkToClose){
        QDialog::accept();
    }
}

