#include <cte/client_new/network_worker.h>

namespace cte {
    NetworkWorker::NetworkWorker(QObject *parent) : QObject(parent) {
        qRegisterMetaType<QSharedPointer<Message>>("QSharedPointer<Message>");
        connect(this, &NetworkWorker::new_server, this, &NetworkWorker::connect_to_server);
    }

    void NetworkWorker::connect_to_server(const QString &hostname, int port) {
        socket_ = new Socket(this);

        connect(socket_, &Socket::connected, this, &NetworkWorker::connected);
        connect(socket_, &Socket::disconnected, this, &NetworkWorker::disconnected);
        connect(socket_, &Socket::ready_message, this, &NetworkWorker::read_message);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))      // errorOccurred() was introduced in Qt 5.15
        connect(socket_.data(), &Socket::errorOccurred, this, &NetworkWorker::error);
#else
        connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&Socket::error), this, &NetworkWorker::error);
#endif

        socket_->connectToHost(hostname, port);
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
}
