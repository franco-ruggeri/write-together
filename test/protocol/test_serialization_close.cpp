/*
 * Author: Franco Ruggeri
 */

#include <protocol/CloseMessage.h>
#include <iostream>
#include <memory>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::shared_ptr<Message> message1, message2;

    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " document username" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // original message
    message1 = std::make_shared<CloseMessage>(argv[1], argv[2]);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}