#include <QtWidgets/QApplication>
#include <cte/client/client.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    const std::string& usage = std::string("usage ") + argv[0] + " hostname port" + "\n\n" +
                               "positional arguments:" + "\n\t" +
                               "hostname" + "\t\t" + "hostname or IP address of the server" + "\n\t" +
                               "port" + "\t" + "TCP port the server is listening on";

    // check number of arguments
    if (argc < 3) {
        std::cout << usage << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // launch client
    try {
        int port = std::stoi(argv[2]);
        QApplication app(argc, argv);
        cte::Client client(argv[1], port);
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
