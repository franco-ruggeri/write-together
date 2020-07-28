/*
 * Test the idempotency property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <crdt/SharedEditor.h>
#include <QtCore/QString>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " text index" << std::endl;
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
    Symbol symbol1 = editor1.local_erase(index);
    Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_erase(symbol1);
    editor1.remote_erase(symbol2);

    text.remove(index, 1);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}