/*
 * Author: Franco Ruggeri
 */

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <cte/server/Server.h>

int main(int argc, char **argv) {
    const QString& usage = QString("usage ") + argv[0] + " port saving_period_ms";

    // check number of arguments
    if (argc < 3) {
        qDebug() << usage;
        std::exit(EXIT_FAILURE);
    }

    // launch server
    try {
        QCoreApplication app(argc, argv);
        int port = std::stoi(argv[1]);
        int saving_period = std::stoi(argv[2]);
        cte::Server server(port, QThread::idealThreadCount(), saving_period);
        return app.exec();
    } catch (const std::invalid_argument& e) {
        qDebug() << e.what();
        qDebug() << "invalid argument";
        qDebug() << usage;
        std::exit(EXIT_FAILURE);
    } catch (const std::exception& e) {
        qDebug() << e.what();
        std::exit(EXIT_FAILURE);
    } catch (...) {
        qDebug() << "unknown exception";
        std::exit(EXIT_FAILURE);
    }
}
