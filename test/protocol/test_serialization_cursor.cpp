/*
 * Author: Franco Ruggeri
 */

#include <protocol/CursorMessage.h>
#include <protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 8 || argv[4][1] != 0) {
        std::cerr << "usage: " << argv[0]
                  << " document_owner document_name username value site_id site_counter position[0] [position[1] ...]"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // fill position
    QVector<int> position;
    for (int i=6; i<argc; i++)
        position.push_back(std::stoi(argv[i]));

    // original message
    message1 = QSharedPointer<CursorMessage>::create(Document(argv[1], argv[2]), argv[3],
                                                     Symbol(argv[4][0], std::stoi(argv[5]), std::stoi(argv[6]),
                                                            position));

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}