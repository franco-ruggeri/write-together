/*
 * Test the idempotency property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <editor/crdt/SharedEditor.h>
#include <editor/crdt/Symbol.h>
#include <QtCore/QString>

int main() {
    QString text = "HAT";
    const int index = 0;

    editor::SharedEditor editor1(editor::SharedEditor::starting_site_id, editor::SharedEditor::starting_site_counter);
    editor::SharedEditor editor2(editor::SharedEditor::starting_site_id + 1, editor::SharedEditor::starting_site_counter);

    // initial text
    for (int i=0; i<text.size(); i++) {
        editor::Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // insert and delete (must commute)
    editor::Symbol symbol1 = editor1.local_erase(index);
    editor::Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_erase(symbol1);
    editor1.remote_erase(symbol2);

    text.remove(index, 1);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}