/*
 * Test the commutativity property required by a collaborative text cte and obtained by means of CRDT.
 *
 * Authors: Antonino Musmeci, Franco Ruggeri
 */

#include <iostream>
#include <QtCore/QString>
#include <editor/crdt/SharedEditor.h>
#include <editor/crdt/Symbol.h>
#include <editor/crdt/Lseq.h>

void print_position(const QVector<int>& position) {
    std::cout << "[ ";
    for (const auto& p : position)
        std::cout << p << " ";
    std::cout << "]" << std::endl;
}

int main() {
    const QString text = "test";
    cte::SharedEditor editor(cte::SharedEditor::starting_site_id, cte::SharedEditor::starting_site_counter, cte::Lseq(1));

    for (int i=0; i<text.size(); i++) {
        cte::Symbol s = editor.local_insert(i, text[i]);
        print_position(s.position());
    }

    for (int i=1; i<text.size()+1; i++) {
        cte::Symbol s = editor.local_insert(i, text[i - 1]);
        print_position(s.position());
    }

    cte::Symbol s = editor.local_insert(2, 'b');

    return 0;
}