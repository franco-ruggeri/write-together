#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <cte/widget/form.h>

namespace Ui {
    class ConnectForm;
}

namespace cte {
    class ConnectForm : public Form {
        Q_OBJECT

        QSharedPointer<Ui::ConnectForm> ui_;

    signals:
        void connect_to_server(const QString& hostname, int port);

    private slots:
        void on_connect_clicked();

    public slots:
        void clear() override;

    public:
        explicit ConnectForm(QWidget *parent=nullptr);
    };
}
