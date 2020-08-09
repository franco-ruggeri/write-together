/*
 * TCP server. It launches serving threads, listens on a port, accepts connections and distributes them among its
 * threads.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QVector>
#include <QtNetwork/QTcpServer>
#include <cte/server/Worker.h>
#include <cte/server/Saver.h>

namespace cte {
    class Server : public QTcpServer {
        Q_OBJECT

        QVector<QSharedPointer<Worker>> workers_;
        QSharedPointer<Saver> saver_;

        void incomingConnection(qintptr socket_fd) override;

    public:
        Server(int port, int n_workers, int saving_interval_ms);
    };
}
