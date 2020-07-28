/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileMessage.h>
#include <QtCore/QSharedPointer>
#include <QtGui/QImage>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 8) {
        std::cerr << "usage: " << argv[0] << " username password width height r g b" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // fill icon
    QImage icon(std::stoi(argv[3]), std::stoi(argv[4]), QImage::Format_RGB32);
    icon.fill(QColor(std::stoi(argv[5]), std::stoi(argv[6]), std::stoi(argv[7])));

    // original message
    message1 = QSharedPointer<ProfileMessage>::create(argv[1], argv[2], icon);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}