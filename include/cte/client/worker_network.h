#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <cte/network/socket.h>
#include <cte/protocol/message.h>

namespace cte {
    class NetworkWorker : public QObject {
        Q_OBJECT

        QPointer<Socket> socket_;
        static const QString ca_certificate_;

    signals:
        void new_server(const QString& hostname, int port);
        void connected();
        void error_occurred(const QString& error);
        void new_message(const QSharedPointer<Message>& message);

    private slots:
        void error_occurred(QAbstractSocket::SocketError socket_error);
        void connect_to_server(const QString& hostname, int port);
        void read_message();

    public slots:
        void set_server(const QString& hostname, int port);
        void send_message(const QSharedPointer<Message>& message);

    public:
        explicit NetworkWorker(QObject *parent=nullptr);
    };
}
