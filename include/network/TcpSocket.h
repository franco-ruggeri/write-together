/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QTcpSocket>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class TcpSocket : public QTcpSocket {
        Q_OBJECT

    public:
        TcpSocket(quintptr socket_fd);
        std::shared_ptr<Message> read_message();
        void write_message(std::shared_ptr<Message> message);
    };
}
