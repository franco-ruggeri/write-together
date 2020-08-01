/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtNetwork/QTcpSocket>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class TcpSocket : public QTcpSocket {
    public:
        TcpSocket(int socket_fd);
        QSharedPointer<Message> read_message();
        void write_message(const QSharedPointer<Message>& message);
    };
}
