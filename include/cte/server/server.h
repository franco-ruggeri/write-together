/*
 * TCP server. It launches serving threads, listens on a port, accepts connections and distributes them among its
 * threads.
 */

#pragma once

#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#include <QtCore/QThread>
#include <QtNetwork/QTcpServer>
#include <cte/server/worker.h>
#include <cte/server/saver.h>

namespace cte {
    class Worker;

    class Server : public QTcpServer {
        Q_OBJECT

        QVector<QSharedPointer<Worker>> workers_;
        QSharedPointer<Saver> saver_;
        QList<QPointer<QThread>> threads_;

        QSslKey private_key_;
        QSslCertificate local_certificate_;

        void incomingConnection(qintptr socket_fd) override;

    public:
        Server(int n_workers, int saving_period);
        ~Server() override;
        void listen(int port);
        QSslKey private_key() const;
        QSslCertificate local_certificate() const;
    };
}
