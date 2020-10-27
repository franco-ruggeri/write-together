/*
 * TCP server. It launches serving threads, listens on a port, accepts connections and distributes them among its
 * threads.
 */

#pragma once

#include <QtCore/QVector>
#include <QtNetwork/QTcpServer>
#include <cte/server/worker.h>
#include <cte/server/saver.h>

namespace cte {
    class Worker;
    class Server : public QTcpServer {
        Q_OBJECT

        QVector<QSharedPointer<Worker>> workers_;
        QSharedPointer<Saver> saver_;

        QSslKey key_;
        QSslCertificate certificate_;

        void incomingConnection(qintptr socket_fd) override;

    public:
        Server(int port, int n_workers, int saving_period);
        QSslKey key() const;
        QSslCertificate certificate() const;
    };
}
