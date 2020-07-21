/*
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "DocumentsMessage.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::vector<QString> documents;
    for (int i=1; i<argc; i++)
        documents.push_back(argv[i]);

    DocumentsMessage message(documents);
    std::cout << message.serialize().toStdString() << std::endl;

    return 0;
}