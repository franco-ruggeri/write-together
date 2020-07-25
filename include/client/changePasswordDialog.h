/*
 * Author: Antonino Musmeci
 */

#pragma once
#include <qdialog.h>
#include "myClient.h"
namespace Ui {
    class changePasswordDialog;
}

class changePasswordDialog :public QDialog {
Q_OBJECT

public:

    Ui::changePasswordDialog *ui;
    changePasswordDialog(QWidget *parent, std::shared_ptr<myClient> client);

private:
    std::shared_ptr<myClient> client;
    bool itsOkToClose = true;
private slots:

    void on_changepass_change_pushButton_clicked();

    void accept() override;
};
