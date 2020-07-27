/*
 * Author: Antonino Musmeci
 */

#include <QStackedWidget>
#include "client/newFileDialog.h"
#include "ui_newFileDialog.h"
#include <QMessageBox>
#include <QtWidgets/QWidget>
#include <client/loginTextEditor.h>

newFileDialog::newFileDialog(QWidget *parent,std::shared_ptr<myClient> client, texteditor *editor):
    QDialog(parent),ui(new Ui::newFileDialog),client(client),editor(editor) {
    ui->setupUi(this);
}


void newFileDialog::on_newfile_create_pushButton_clicked() {
    QString filename = ui->newfile_filename_lineEdit->text();
    ui->newfile_filename_lineEdit->clear();

    std::tuple<bool,QString>response = client->new_file(filename);
    if(get<0>(response)){
        QMessageBox::information(this, "FILE CREATED", get<1>(response));
        emit open_editor(filename);
    }
    else{
        QMessageBox::warning(this, "ERROR", get<1>(response));
    }
    emit reject();
}

void newFileDialog::on_newfile_cancel_pushButton_clicked(){
    emit reject();
}

