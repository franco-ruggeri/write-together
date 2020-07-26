/*
 * Author: Antonino Musmeci
 */

#include <QStackedWidget>
#include <client/myClient.h>
#include <QMessageBox>
#include "client/changeUsernameDialog.h"
#include "ui_changeUsernameDialog.h"

changeUsernameDialog::changeUsernameDialog(QWidget *parent, std::shared_ptr<myClient> client):
        QDialog(parent),ui(new Ui::changeUsernameDialog), client(client){

    ui->setupUi(this);
    ui->changeuser_currentuser_label->setText("current username: " + client->user->username());



}


void changeUsernameDialog::on_changeuser_change_pushButton_clicked() {

    qDebug("pressed");
    QString new_username = ui->changeuser_newuser_lineEdit->text();

    if (!utility::check_username_validity(new_username)) {
        QMessageBox::warning(this, "WARNING", "Username should be at least 3 characters long");
        itsOkToClose = false;
    }
    else {
        bool response = client->change_username(new_username);

        if (response) {
            QMessageBox::information(this, "USERNAME CHANGED", "Your username has been changed");
            itsOkToClose = true;
        }
        else {
            QMessageBox::warning(this, "WARNING", "ERROR");
            itsOkToClose = true;
        }

    }
    emit accept();

}
void changeUsernameDialog::on_changeuser_cancel_pushButton_clicked() {
    emit reject();
}

#include <iostream>
void changeUsernameDialog::accept(){
    if (itsOkToClose){
        QDialog::reject();
    }
}


