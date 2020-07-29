/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QVector>
#include <QtNetwork/QTcpServer>
#include "Worker.h"

class Server : public QTcpServer {
    Q_OBJECT

    QVector<QSharedPointer<Worker>> workers_;

    void incomingConnection(qintptr socket_fd) override;
public:
    Server(unsigned int port, unsigned int n_threads);
};
