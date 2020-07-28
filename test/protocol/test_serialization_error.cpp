/*
 * Author: Franco Ruggeri
 */

#include <protocol/ErrorMessage.h>
#include <QtCore/QSharedPointer>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " reason" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // original message
    message1 = QSharedPointer<ErrorMessage>::create(argv[1]);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}