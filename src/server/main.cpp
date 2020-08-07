/*
 * Author: Franco Ruggeri
 */

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <cte/server/Server.h>

int main(int argc, char **argv) {
    const QString& usage = QString("usage ") + argv[0] + " port milliseconds";

    QCoreApplication app(argc, argv);

    // check arguments
    if (argc < 3) {
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // parse port
    int port;
    try {
        port = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        qDebug() << "invalid port";
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // parse port
    int saving_interval_ms;
    try {
        saving_interval_ms = std::stoi(argv[2]);
    } catch (const std::invalid_argument& e) {
        qDebug() << "invalid saving interval";
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // launch server
    cte::Server server(port, QThread::idealThreadCount(), saving_interval_ms);

    return app.exec();
}
