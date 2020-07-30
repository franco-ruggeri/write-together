/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileMessage.h>
#include <protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtGui/QImage>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 10) {
        std::cerr << "usage: " << argv[0] << " username name surname password width height r g b" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // fill icon
    QImage icon(std::stoi(argv[5]), std::stoi(argv[6]), QImage::Format_RGB32);
    icon.fill(QColor(std::stoi(argv[7]), std::stoi(argv[8]), std::stoi(argv[9])));

    // original message
    message1 = QSharedPointer<ProfileMessage>::create(Profile(argv[1], argv[2], argv[3], icon), argv[4]);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}