/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "ErrorMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " reason" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    ErrorMessage message(argv[1]);
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}