/*
 * Author: Antonino Musmeci
 */


#pragma once

#include <qdialog.h>
#include <cte/client/texteditor.h>

namespace Ui {
    class NewFileDialog;
}

class NewFileDialog:public QDialog {
    Q_OBJECT


public:

    Ui::NewFileDialog *ui;
    NewFileDialog(QWidget *parent, QSharedPointer<myClient> client, QSharedPointer<texteditor> editor);

private:
    QSharedPointer<texteditor> editor;
    QSharedPointer<myClient> client;

private slots:
    void on_newfile_create_pushButton_clicked();
    void on_newfile_cancel_pushButton_clicked();

signals:
   void open_editor(fileInfo file);
};


