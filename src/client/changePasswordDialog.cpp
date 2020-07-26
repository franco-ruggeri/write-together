/*
 * Author: Antonino Musmeci
 */

#include <QStackedWidget>
#include <client/myClient.h>
#include <QMessageBox>
#include "client/changePasswordDialog.h"
#include "ui_changePasswordDialog.h"

changePasswordDialog::changePasswordDialog(QWidget *parent, std::shared_ptr<myClient> client):
        QDialog(parent),ui(new Ui::changePasswordDialog), client(client){
    ui->setupUi(this);
    ui->changepass_confirmpass_lineEdit->setEchoMode(QLineEdit::Password);
    ui->changepass_newpass_lineEdit->setEchoMode(QLineEdit::Password);

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
}

#include <iostream>
void changePasswordDialog::accept(){
    if (itsOkToClose){
        QDialog::reject();
    }
}

