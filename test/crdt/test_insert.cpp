/*
 * Test the commutativity property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Authors: Antonino Musmeci, Franco Ruggeri
 */

#include <iostream>
#include <crdt/SharedEditor.h>
#include <crdt/Lseq.h>

using namespace collaborative_text_editor;

void print_position(const QVector<int>& position) {
    std::cout << "[ ";
    for (const auto& p : position)
        std::cout << p << " ";
    std::cout << "]" << std::endl;
}

int main() {
    Lseq pos_allocator(1);
    SharedEditor editor(1, pos_allocator);

    std::string text("test");
    for (int i=0; i<text.size(); i++) {
        Symbol s = editor.local_insert(i, text[i]);
        print_position(s.position());
    }

    for (int i=1; i<text.size()+1; i++) {
        Symbol s = editor.local_insert(i, text[i-1]);
        print_position(s.position());
    }

    Symbol s = editor.local_insert(2, 'b');

    return 0;
}