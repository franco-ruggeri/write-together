/*
 * Author: Stefano Di Blasio, Claudio Nuzzo
 */

#include <cte/client/OpenFileUrl.h>
#include <ui_OpenFileUrl.h>
#include <QtCore/QMimeData>
#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QMessageBox>
#include <QTStylesheet.h>


namespace cte {
    OpenFileUrl::OpenFileUrl(QWidget *parent, QSharedPointer<myClient> client) :
            QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint), ui(new Ui::OpenFileUrl), client_(client) {
        ui->setupUi(this);

        ui->cancel_button->setStyleSheet(btnCancelFileStylesheet);
        ui->paste_button->setStyleSheet(btnPasteFileStylesheet);
        ui->open_button->setStyleSheet(btnCreateFileStylesheet);
#ifndef QT_NO_CLIPBOARD
        /* paste button enabled at opening */
        const QMimeData *md = QGuiApplication::clipboard()->mimeData();
        if (md) ui->paste_button->setEnabled(md->hasText());
        /* paste button to be enabled or disabled due to clipboard changes */
        QObject::connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &OpenFileUrl::clipboard_changed);
#endif
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

#ifndef QT_NO_CLIPBOARD
    void OpenFileUrl::clipboard_changed() {
        const QMimeData *md = QGuiApplication::clipboard()->mimeData();
        if (md) {
            ui->paste_button->setEnabled(md->hasText());
        }
    }
#endif

/*
    TODO: understand if correct to delete ui
    OpenFileUrl::~OpenFileUrl() noexcept {
        if (ui) delete ui;
        QDialog::~QDialog();
    }
*/
}