/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <qdialog.h>
#include <cte/client/myClient.h>

namespace Ui {
    class changeUsernameDialog;
}

class changeUsernameDialog :public QDialog {
Q_OBJECT

public:

    Ui::changeUsernameDialog *ui;
    changeUsernameDialog(QWidget *parent, QSharedPointer<myClient> client);

private:
    QSharedPointer<myClient> client;
    bool itsOkToClose = true;
private slots:

    void on_changeuser_change_pushButton_clicked();
    void on_changeuser_cancel_pushButton_clicked();

    void accept() override;
};
