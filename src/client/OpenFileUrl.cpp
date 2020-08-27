/*
 * Author: Stefano Di Blasio
 */

#include <cte/client/OpenFileUrl.h>
#include <ui_OpenFileUrl.h>
#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QMessageBox>

namespace cte {
    OpenFileUrl::OpenFileUrl(QWidget *parent, QSharedPointer<myClient> client) :
            QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), ui(new Ui::OpenFileUrl), client_(client) {
        ui->setupUi(this);
    }

    void OpenFileUrl::clear_content() {
        ui->file_url_lineEdit->clear();
    }

    void OpenFileUrl::on_cancel_button_clicked() {
        ui->file_url_lineEdit->clear();
        reject();
    }

    void OpenFileUrl::on_paste_button_clicked() {
        QClipboard *cb = QGuiApplication::clipboard();
        ui->file_url_lineEdit->setText(cb->text());
    }

    void OpenFileUrl::on_open_button_clicked() {
        QString file_url = ui->file_url_lineEdit->text();
        if (file_url.isEmpty()) {
            QMessageBox::warning(this, tr("Incomplete fields"), tr("Please provide a valuable sharing link."));
            reject();
            return;
        }
        client_->open_file(file_url, false);
        accept();
    }
}