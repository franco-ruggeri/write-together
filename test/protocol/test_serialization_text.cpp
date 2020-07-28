/*
 * Author: Franco Ruggeri
 */

#include <protocol/TextMessage.h>
#include <QtCore/QSharedPointer>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " document text" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // original message
    message1 = QSharedPointer<TextMessage>::create(argv[1], argv[2]);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}