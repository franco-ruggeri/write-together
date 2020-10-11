/*
 * Author: Stefano Di Blasio
 */

#pragma once

#include <QtWidgets/QDialog>
#include <cte/client/myClient.h>

namespace Ui {
    class OpenFileUrl;
}

namespace cte {
    class OpenFileUrl : public QDialog {
        Q_OBJECT

        QSharedPointer<myClient> client_;
        Ui::OpenFileUrl *ui;

    public:
        OpenFileUrl(QWidget* parent, QSharedPointer<myClient> client);
        // virtual ~OpenFileUrl() noexcept;
        void clear_content();

    private slots:
        void on_cancel_button_clicked();
        void on_paste_button_clicked();
        void on_open_button_clicked();

#ifndef QT_NO_CLIPBOARD
        void clipboard_changed();
#endif
    };
}