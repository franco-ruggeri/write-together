/*
 * Author: Franco Ruggeri
 */

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <cte/server/Server.h>

int parse_int(const char *arg, const QString& error) {
    int result;
    try {
        result = std::stoi(arg);
    } catch (const std::invalid_argument& e) {
        qDebug() << error;
        QCoreApplication::exit(EXIT_FAILURE);
    }
    return result;
}

int main(int argc, char **argv) {
    const QString& usage = QString("usage ") + argv[0] + " port saving_period_ms";
    QCoreApplication app(argc, argv);

    // check number of arguments
    if (argc < 3) {
        qDebug() << usage;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    // parse arguments
    int port = parse_int(argv[1], "invalid port");
    int saving_period = parse_int(argv[2], "invalid saving period");

    // launch server
    cte::Server server(port, QThread::idealThreadCount(), saving_period);
    return app.exec();
}
