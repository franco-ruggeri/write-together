/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include <QCoreApplication>
#include <QRunnable>
#include <QTcpServer>
#include <QThreadPool>


class Task : public QRunnable {
    quintptr socket_fd_;

public:
    Task(unsigned int socket_fd) : socket_fd_(socket_fd) {}
};

class Server : public QTcpServer {
    void incomingConnection(qintptr socket_fd) override {
        Task *task = new Task(socket_fd);
        QThreadPool::globalInstance()->start(task);
    }

public:
    Server(unsigned int port) {
        if (listen(QHostAddress::Any, port)) {
            qDebug() << "listening on port" << this->serverPort();
        } else {
            qDebug() << "listen() failed";
            QCoreApplication::exit(EXIT_FAILURE);
        }
    }
};





int main(int argc, char **argv) {
    const std::string usage = std::string{} + "usage: " + argv[0] + " port";

    if (argc < 2) {
        std::cout << usage << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // parse port
    unsigned int port;
    try {
        port = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "invalid port" << std::endl;
        std::cout << usage << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // launch server
    QCoreApplication app(argc, argv);
    Server server(port);
    app.exec();
}