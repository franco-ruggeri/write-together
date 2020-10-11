/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QQueue>
#include <QtNetwork/QSslSocket>
#include <cte/protocol/message.h>

namespace cte {
    class Socket : public QSslSocket {
        Q_OBJECT

        QQueue<QByteArray> lines_;

    private slots:
        void read_lines();

    signals:
        void ready_message();

    public:
        Socket();
        void set_socket_descriptor(int socket_fd);
        QSharedPointer<Message> read_message();
        void write_message(const QSharedPointer<Message>& message);
    };
}
