/*
 * Test the commutativity property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <crdt/SharedEditor.h>
#include <QString>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 3 || argv[2][1] != 0) {
        std::cerr << "usage: " << argv[0] << " text index value" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    SharedEditor editor1(1);
    SharedEditor editor2(2);

    // initial text
    QString text(argv[1]);
    for (int i=0; i<text.size(); i++) {
        Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // insert and delete (must commute)
    int index = std::stoi(argv[2]);
    char value = argv[3][0];
    Symbol symbol1 = editor1.local_insert(index, value);
    Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_insert(symbol1);
    editor1.remote_erase(symbol2);

    text[index] = value;
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}