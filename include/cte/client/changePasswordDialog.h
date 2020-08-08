/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <qdialog.h>
#include <cte/client/myClient.h>

namespace Ui {
    class changePasswordDialog;
}

class changePasswordDialog :public QDialog {
Q_OBJECT

public:

    Ui::changePasswordDialog *ui;
    changePasswordDialog(QWidget *parent, QSharedPointer<myClient> client);

private:
    QSharedPointer<myClient> client;
    bool itsOkToClose = true;
private slots:

    void on_changepass_change_pushButton_clicked();
    void on_changepass_cancel_pushButton_clicked();


    void accept() override;

};
