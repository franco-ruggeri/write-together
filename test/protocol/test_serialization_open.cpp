/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "OpenMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " document" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    OpenMessage message(argv[1]);
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}