/*
 * Test the out-of-order messages problem that can happen in peer-to-peer architecture or in multi-threaded server.
 *
 * Author: Franco Ruggeri
 */

#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QString>
#include <QtCore/QChar>
#include <assert.h>

int main() {
    cte::SharedEditor editor1(cte::SharedEditor::starting_site_id);
    cte::SharedEditor editor2(cte::SharedEditor::starting_site_id + 1);
    cte::SharedEditor editor3(cte::SharedEditor::starting_site_id + 2);

    cte::Symbol s1 = editor1.local_insert(0, 'H');
    editor2.remote_insert(s1);
    cte::Symbol s2 = editor2.local_erase(0);
    editor1.remote_erase(s2);
    editor3.remote_erase(s2);       // erase (from editor2) arrives before insert (from editor1)
    editor3.remote_insert(s1);      // now insert (from editor1) arrives, but this symbol should be deleted

    assert(editor1.to_string() == "");
    assert(editor2.to_string() == "");
    assert(editor3.to_string() == "");

    return 0;
}