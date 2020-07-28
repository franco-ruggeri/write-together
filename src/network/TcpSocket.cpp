/*
 * Author: Franco Ruggeri
 */

#include <network/TcpSocket.h>

namespace collaborative_text_editor {
    TcpSocket::TcpSocket(quintptr socket_fd) {
        setSocketDescriptor(socket_fd);
    }

    QSharedPointer<Message> TcpSocket::read_message() {
        QByteArray bytes = readLine();
        bytes.remove(bytes.size()-1, 1);        // remove '\n'
        if (bytes.at(bytes.size()-1) == '\r')
            bytes.remove(bytes.size()-1, 1);    // remove '\r', if present (e.g. telnet)
        return Message::deserialize(bytes);
    }

    void TcpSocket::write_message(QSharedPointer<Message> message) {
        QByteArray bytes = message->serialize();
        bytes.push_back('\n');
        write(bytes);
    }
}
