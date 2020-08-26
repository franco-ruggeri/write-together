/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <qdialog.h>
#include <cte/client/myClient.h>

namespace Ui {
    class ProfileUpdateDialog;
}

class ProfileUpdateDialog : public QDialog {
Q_OBJECT

public:

    Ui::ProfileUpdateDialog *ui;
    ProfileUpdateDialog(QWidget *parent, QSharedPointer<myClient> client);

private:
    QSharedPointer<myClient> client;
    bool itsOkToClose = true;
private slots:

    void on_change_icon_pushButton_clicked();
    void on_changepass_change_pushButton_clicked();
    void on_changepass_cancel_pushButton_clicked();

    void update_profile_result(bool result, const QString& error_message);
    void accept() override;

};
