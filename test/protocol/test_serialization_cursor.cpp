/*
 * Author: Franco Ruggeri
 */

#include <protocol/CursorMessage.h>
#include <iostream>
#include <memory>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::shared_ptr<Message> message1, message2;

    if (argc < 7 || argv[3][1] != 0) {
        std::cerr << "usage: " << argv[0] << " document username value site_id site_counter position[0] [position[1] ...]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // fill position
    std::vector<int> position;
    for (int i=6; i<argc; i++)
        position.push_back(std::stoi(argv[i]));

    // original message
    message1 = std::make_shared<CursorMessage>(argv[1], argv[2], Symbol(argv[3][0], std::stoi(argv[4]),
                                                                        std::stoi(argv[5]), position));

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}