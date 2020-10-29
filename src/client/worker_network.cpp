#include <cte/client/worker_network.h>

namespace cte {
    const QString NetworkWorker::ca_certificate_ = ":/ca_certificate";

    NetworkWorker::NetworkWorker(QObject *parent) : QObject(parent) {
        qRegisterMetaType<QSharedPointer<Message>>("QSharedPointer<Message>");
        connect(this, &NetworkWorker::new_server, this, &NetworkWorker::connect_to_server);
    }

    void NetworkWorker::connect_to_server(const QString &hostname, int port) {
        socket_ = new Socket(this);

        connect(socket_, &Socket::connected, this, &NetworkWorker::connected);
        connect(socket_, &Socket::ready_message, this, &NetworkWorker::read_message);
        connect(socket_, &Socket::disconnected, socket_, &Socket::deleteLater);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))      // errorOccurred() was introduced in Qt 5.15
        connect(socket_, &Socket::errorOccurred,
                this, qOverload<QAbstractSocket::SocketError>(&NetworkWorker::error_occurred));
#else
        connect(socket_, qOverload<QAbstractSocket::SocketError>(&Socket::error),
                this, qOverload<QAbstractSocket::SocketError>(&NetworkWorker::error_occurred));
#endif

        socket_->connect_to_server(hostname, port, ca_certificate_);
    }

    void NetworkWorker::read_message() {
        QSharedPointer<Message> message = socket_->read_message();
        emit new_message(message);
    }

    void NetworkWorker::set_server(const QString& hostname, int port) {
        /*
         * The socket is not created directly here because it must belong to the thread the worker is running on.
         * By means of the signal-slot mechanism, the socket will be created in create_socket() by the correct thread.
         */
        emit new_server(hostname, port);
    }

    void NetworkWorker::send_message(const QSharedPointer<Message>& message) {
        socket_->write_message(message);
    }

    void NetworkWorker::error_occurred(QAbstractSocket::SocketError socket_error) {
        QString error = tr("Socket error. Error code: ") + QString::number(static_cast<int>(socket_error));
        emit error_occurred(error);
    }
}
