/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include <cstring>
#include "InsertMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 6 || argv[2][1] != 0) {
        std::cerr << "usage: " << argv[0] << " document value site_id site_counter position[0] [position[1] ...]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<int> position;
    for (int i=5; i<argc; i++)
        position.push_back(std::stoi(argv[i]));

    InsertMessage message(argv[1], Symbol(argv[2][0], std::stoi(argv[3]), std::stoi(argv[4]), position));
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}