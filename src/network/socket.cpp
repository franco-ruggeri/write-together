/*
 * Author: Franco Ruggeri
 */

#include <cte/network/socket.h>

namespace cte {
    Socket::Socket(QObject *parent) : QSslSocket(parent) {
        connect(this, &Socket::readyRead, this, &Socket::read_lines);
    }

    void Socket::set_socket_descriptor(int socket_fd) {
        if (!QSslSocket::setSocketDescriptor(socket_fd))
            throw std::runtime_error("setSocketDescriptor() failed");
    }

    void Socket::read_lines() {
        // buffer lines
        while (canReadLine()) {
            // read line
            QByteArray bytes = readLine();
            if (bytes.at(bytes.size()-1) != '\n')
                throw std::runtime_error("read() failed");

            // remove '\n' and, if present, '\r'
            bytes.remove(bytes.size()-1, 1);
            if (bytes.at(bytes.size()-1) == '\r')
                bytes.remove(bytes.size()-1, 1);

            // buffer line
            lines_.enqueue(bytes);
            emit ready_message();
        }
    }

    QSharedPointer<Message> Socket::read_message() {
        return Message::deserialize(lines_.dequeue());
    }

    void Socket::write_message(const QSharedPointer<Message>& message) {
        QByteArray bytes = message->serialize();
        bytes.push_back('\n');

        qint64 n_written = write(bytes);
        if (n_written < bytes.size()) throw std::runtime_error("write() failed");
    }
}
