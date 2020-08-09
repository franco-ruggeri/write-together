/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtNetwork/QSslSocket>
#include <cte/protocol/Message.h>

namespace cte {
    class Socket : public QSslSocket {
        Q_OBJECT

    private slots:
        void check_message_ready();

    signals:
        void ready_message();

    public:
        Socket();
        explicit Socket(int socket_fd);
        QSharedPointer<Message> read_message();
        void write_message(const QSharedPointer<Message>& message);
    };
}
