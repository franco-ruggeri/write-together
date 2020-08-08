/*
 * Author: Franco Ruggeri
 */

#include <cte/network/Socket.h>

namespace cte {
    Socket::Socket(int socket_fd) {
        if (!setSocketDescriptor(socket_fd))
            throw std::runtime_error("setSocketDescriptor() failed");
        connect(this, &Socket::readyRead, this, &Socket::check_message_ready);
    }

    void Socket::check_message_ready() {
        if (canReadLine()) emit ready_message();
    }

    QSharedPointer<Message> Socket::read_message() {
        QByteArray bytes = readLine();

        // check result
        if (bytes.at(bytes.size()-1) != '\n')
            throw std::runtime_error("read() failed");

        // remove '\n' and, if present, '\r'
        bytes.remove(bytes.size()-1, 1);
        if (bytes.at(bytes.size()-1) == '\r')
            bytes.remove(bytes.size()-1, 1);

        return Message::deserialize(bytes);
    }

    void Socket::write_message(const QSharedPointer<Message>& message) {
        QByteArray bytes = message->serialize();
        bytes.push_back('\n');

        qint64 n_written = write(bytes);
        if (n_written < bytes.size()) throw std::runtime_error("write() failed");
    }
}
