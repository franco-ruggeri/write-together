/*
 * Author: Antonino Musmeci
 */

#include <QStackedWidget>
#include <cte/client/NewFileDialog.h>
#include "ui_newFileDialog.h"
#include <QMessageBox>
#include <QtWidgets/QWidget>
#include <cte/client/loginTextEditor.h>

NewFileDialog::NewFileDialog(QWidget *parent,QSharedPointer<myClient> client, QSharedPointer<texteditor> editor):
        QDialog(parent), ui(new Ui::NewFileDialog), client(client), editor(editor) {
    ui->setupUi(this);
}


void NewFileDialog::on_newfile_create_pushButton_clicked() {
    QString filename = ui->newfile_filename_lineEdit->text();
    ui->newfile_filename_lineEdit->clear();

    std::optional<fileInfo> response =  client->new_file(filename);
        if(response){
            QMessageBox::information(this, "FILE CREATED", "file " + filename + " create" );
            this->hide();
            emit open_editor(*response);
        }
        else{
            QMessageBox::warning(this, "ERROR", "unable to create file " + filename);
        }
    emit reject();
}

void NewFileDialog::on_newfile_cancel_pushButton_clicked(){
    emit reject();
}

