/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include <QByteArray>
#include <QImage>
#include <QBuffer>
#include "ProfileMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 8) {
        std::cerr << "usage: " << argv[0] << " username password width height r g b" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    QImage icon(std::stoi(argv[3]), std::stoi(argv[4]), QImage::Format_RGB32);
    icon.fill(QColor(std::stoi(argv[5]), std::stoi(argv[6]), std::stoi(argv[7])));

    ProfileMessage message(argv[1], argv[2], icon);
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}