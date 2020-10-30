#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QString>
#include <QtCore/QChar>
#include <cassert>


#include <QDebug>

int main() {
    QString text = "CAT";

    cte::SharedEditor editor1(cte::SharedEditor::starting_site_id);
    cte::SharedEditor editor2(cte::SharedEditor::starting_site_id + 1);

    cte::Symbol s;
    int index;
    std::optional<int> index_result;

    // initial text
    for (int i=0; i<text.size(); i++) {
        cte::Symbol s = editor1.local_insert(i, text[i]);
        editor2.remote_insert(s);
    }

    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // erase in head
    index = 0;
    s = editor1.local_erase(index);
    index_result = editor2.remote_erase(s);

    assert(s.value() == text[index]);
    assert(index_result.has_value());
    assert(*index_result == index);
    text.remove(index, 1);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    // erase in tail
    index = 1;
    s = editor2.local_erase(index);
    index_result = editor1.remote_erase(s);

    assert(s.value() == text[index]);
    assert(index_result.has_value());
    assert(*index_result == index);
    text.remove(index, 1);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}