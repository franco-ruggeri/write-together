#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtWidgets/QWidget>

namespace Ui {
    class ConnectionForm;
}

namespace cte {
    class ConnectionForm : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::ConnectionForm> ui_;

    signals:
        void connection_request(const QString& hostname, int port);

    private slots:
        void on_connect_clicked();

    public slots:
        void clear();

    public:
        explicit ConnectionForm(QWidget *parent=nullptr);
    };
}
