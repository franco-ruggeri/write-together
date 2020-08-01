/*
 * Author: Antonino Musmeci
 */


#pragma once
#include <qdialog.h>
#include "texteditor.h"

namespace Ui {
    class newFileDialog;
}

class newFileDialog:public QDialog {
    Q_OBJECT


public:

    Ui::newFileDialog *ui;
    newFileDialog(QWidget *parent, QSharedPointer<myClient> client, QSharedPointer<texteditor> editor);

private:
    QSharedPointer<texteditor> editor;
    QSharedPointer<myClient> client;

private slots:
    void on_newfile_create_pushButton_clicked();
    void on_newfile_cancel_pushButton_clicked();

signals:
   void open_editor(fileInfo file);
};


