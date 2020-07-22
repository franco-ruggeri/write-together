/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "ProfileOkMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    ProfileOkMessage message;
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}