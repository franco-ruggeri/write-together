/*
 * Author: Franco Ruggeri
 */

#include <QCoreApplication>
#include "Server.h"

int main(int argc, char **argv) {
    const QString& usage = QString("usage") + argv[0] + " port";
    const unsigned int n_threads = 50;

    QCoreApplication app(argc, argv);

    // check arguments
    if (argc < 2) {
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // parse port
    unsigned int port;
    try {
        port = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        qDebug() << "invalid port";
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // launch server
    Server server(port, n_threads);

    return app.exec();
}