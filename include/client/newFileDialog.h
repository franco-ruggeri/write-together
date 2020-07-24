#ifndef TEXTEDITORPROJECTS_NEWFILEDIALOG_H
#define TEXTEDITORPROJECTS_NEWFILEDIALOG_H
#include <qdialog.h>
#include "texteditor.h"

namespace Ui {
    class newFileDialog;
}

class newFileDialog:public QDialog {
    Q_OBJECT


public:

    Ui::newFileDialog *ui;
    newFileDialog(QWidget *parent, std::shared_ptr<myClient> client, texteditor *editor);

private:
    texteditor *editor = nullptr;
    std::shared_ptr<myClient> client;

private slots:
    void on_newfile_create_pushButton_clicked();

};


#endif //TEXTEDITORPROJECTS_NEWFILEDIALOG_H