/*
 * Test the commutativity property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <iostream>
#include "SharedEditor.h"

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 3 || argv[2][1] != 0) {
        std::cerr << "usage: " << argv[0] << " text insert_value index" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    SharedEditor editor1(1);
    SharedEditor editor2(2);

    // initial text
    std::string initial_text(argv[1]);
    for (int i=0; i<initial_text.size(); i++) {
        Symbol s = editor1.local_insert(i, initial_text[i]);
        editor2.remote_insert(s);
    }

    std::cout << editor1.to_string().toStdString() << std::endl;
    std::cout << editor2.to_string().toStdString() << std::endl;

    // insert and delete (must commute)
    int index = std::stoi(argv[2]);
    Symbol symbol1 = editor1.local_insert(index, argv[3][0]);
    Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_insert(symbol1);
    editor1.remote_erase(symbol2);

    std::cout << editor1.to_string().toStdString() << std::endl;
    std::cout << editor2.to_string().toStdString() << std::endl;

    return 0;
}