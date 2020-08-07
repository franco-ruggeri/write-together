/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtNetwork/QTcpSocket>
#include <cte/protocol/Message.h>

namespace cte {
    class TcpSocket : public QTcpSocket {
        Q_OBJECT

    public:
        explicit TcpSocket(int socket_fd);
        QSharedPointer<Message> read_message();
        void write_message(const QSharedPointer<Message>& message);
    };
}
