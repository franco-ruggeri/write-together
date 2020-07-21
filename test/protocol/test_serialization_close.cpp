/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "CloseMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " document username" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    CloseMessage message(argv[1], argv[2]);
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}