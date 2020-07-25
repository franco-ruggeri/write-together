/*
 * Author: Antonino Musmeci
 */

#include <QStackedWidget>
#include "client/newFileDialog.h"
#include "ui_newFileDialog.h"

newFileDialog::newFileDialog(QWidget *parent,std::shared_ptr<myClient> client, texteditor *editor):
    QDialog(parent),ui(new Ui::newFileDialog),client(client),editor(editor) {
    ui->setupUi(this);

}


void newFileDialog::on_newfile_create_pushButton_clicked() {
    QString filename = ui->newfile_filename_lineEdit->text();
    ui->newfile_filename_lineEdit->clear();
    fileInfo file(filename,"");
    editor = new texteditor(nullptr, client,file);
    client->new_file(ui->newfile_filename_lineEdit->text());
    this->parentWidget()->hide();
    editor->show();

}


