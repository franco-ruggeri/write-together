/*
 * Author: Franco Ruggeri
 */

#include <protocol/InsertMessage.h>
#include <iostream>
#include <memory>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::shared_ptr<Message> message1, message2;

    if (argc < 6 || argv[2][1] != 0) {
        std::cerr << "usage: " << argv[0] << " document value site_id site_counter position[0] [position[1] ...]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // fill position
    std::vector<int> position;
    for (int i=6; i<argc; i++)
        position.push_back(std::stoi(argv[i]));

    // original message
    message1 = std::make_shared<InsertMessage>(argv[1], Symbol(argv[2][0], std::stoi(argv[3]),
                                                               std::stoi(argv[4]), position));

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}