#pragma once

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QQueue>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
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
        explicit Socket(QObject *parent=nullptr);
        void setup_server(int socket_fd, const QSslCertificate& local_certificate, const QSslKey& private_key);
        void connect_to_server(const QString& hostname, int port, const QString& ca_certificate);
        QSharedPointer<Message> read_message();
        void write_message(const QSharedPointer<Message>& message);
    };
}
