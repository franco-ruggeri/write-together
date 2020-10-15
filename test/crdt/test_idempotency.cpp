/*
 * Test the idempotency property required by a collaborative text editor and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QString>
#include <assert.h>

int main() {
    QString text = "HAT";
    const int index = 0;

    cte::SharedEditor editor1(cte::SharedEditor::starting_site_id);
    cte::SharedEditor editor2(cte::SharedEditor::starting_site_id + 1);

    // initial text
    for (int i=0; i<text.size(); i++) {
        cte::Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // delete (in both editors, must be idempotent)
    cte::Symbol symbol1 = editor1.local_erase(index);
    cte::Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_erase(symbol1);
    editor1.remote_erase(symbol2);

    text.remove(index, 1);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}