#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <cte/server/server.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    const std::string& usage = std::string("usage ") + argv[0] + " port saving_period" + "\n\n" +
            "positional arguments:" + "\n\t" +
            "port" + "\t\t" + "TCP port to use for listening" + "\n\t" +
            "saving_period" + "\t" + "period for saving documents (in milliseconds)";

    // check number of arguments
    if (argc < 3) {
        std::cout << usage << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // launch server
    try {
        int port = std::stoi(argv[1]);
        int saving_period = std::stoi(argv[2]);
        QCoreApplication app(argc, argv);
        cte::Server server(QThread::idealThreadCount(), saving_period);
        server.listen(port);
        return app.exec();
    } catch (const std::invalid_argument& e) {
        qDebug() << e.what();
        std::cout << "invalid argument\n\n" << usage << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        qDebug() << e.what();
        return EXIT_FAILURE;
    } catch (...) {
        qDebug() << "unknown exception";
        return EXIT_FAILURE;
    }
}
