/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "LogoutMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    LogoutMessage message;
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}