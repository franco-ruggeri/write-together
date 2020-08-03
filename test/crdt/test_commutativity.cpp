/*
 * Test the commutativity property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <editor/crdt/SharedEditor.h>
#include <QtCore/QString>
#include <QtCore/QChar>

using namespace editor;

int main() {
    QString text = "CAT";
    const int index = 1;
    const QChar value = 'H';

    SharedEditor editor1(SharedEditor::start_site_id, SharedEditor::start_site_counter);
    SharedEditor editor2(SharedEditor::start_site_id+1, SharedEditor::start_site_counter);

    // initial text
    for (int i=0; i<text.size(); i++) {
        Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // insert and delete (must commute)
    Symbol symbol1 = editor1.local_insert(index, value);
    Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_insert(symbol1);
    editor1.remote_erase(symbol2);

    text[index] = value;
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}