/*
 * Author: Franco Ruggeri
 */

#include <network/TcpSocket.h>

namespace collaborative_text_editor {
    TcpSocket::TcpSocket(quintptr socket_fd) {
        if (!setSocketDescriptor(socket_fd))
            throw std::runtime_error("setSocketDescriptor() failed");
    }

    QSharedPointer<Message> TcpSocket::read_message() {
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

    void TcpSocket::write_message(const QSharedPointer<Message>& message) {
        QByteArray bytes = message->serialize();
        bytes.push_back('\n');
        if (write(bytes) == -1)
            throw std::runtime_error("write() failed");
    }
}
