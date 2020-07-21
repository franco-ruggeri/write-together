/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include <cstring>
#include "CursorMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 7 || argv[3][1] != 0) {
        std::cerr << "usage: " << argv[0] << " document username value site_id site_counter position[0] [position[1] ...]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<int> position;
    for (int i=6; i<argc; i++)
        position.push_back(std::stoi(argv[i]));

    CursorMessage message(argv[1], argv[2], Symbol(argv[3][0], std::stoi(argv[4]), std::stoi(argv[5]), position));
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}