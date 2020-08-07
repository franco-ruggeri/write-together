/*
 * Test the commutativity property required by a collaborative text cte and obtained by means of CRDT.
 *
 * Author: Franco Ruggeri
 */

#include <cte/crdt/SharedEditor.h>
#include <cte/crdt/Symbol.h>
#include <QtCore/QString>
#include <QtCore/QChar>

#include <iostream>

int main() {
    QString text = "CAT";
    const int index = 1;
    const QChar value = 'H';

    cte::SharedEditor editor1(cte::SharedEditor::starting_site_id);
    cte::SharedEditor editor2(cte::SharedEditor::starting_site_id + 1);

    // initial text
    for (int i=0; i<text.size(); i++) {
        cte::Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // insert and delete (must commute)
    cte::Symbol symbol1 = editor1.local_insert(index, value);
    cte::Symbol symbol2 = editor2.local_erase(index);
    editor2.remote_insert(symbol1);
    editor1.remote_erase(symbol2);

    std::cout << editor1.to_string().toStdString() << std::endl;
    std::cout << editor2.to_string().toStdString() << std::endl;

    text[index] = value;
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}