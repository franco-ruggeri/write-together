#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QChar>
#include <cassert>


#include <QDebug>

int main() {
    const QString text = "CA";
    const int site_id_1 = cte::SharedEditor::starting_site_id+1;
    const int site_id_2 = cte::SharedEditor::starting_site_id+2;

    cte::SharedEditor editor1(site_id_1);
    cte::SharedEditor editor2(site_id_2);

    cte::Symbol s;
    int index;
    std::optional<int> index_result;

    // insert in head
    index = 0;
    s = editor1.local_insert(0, text[index]);
    index_result = editor2.remote_insert(s);

    assert(s.value() == text[index]);
    assert(s.site_id() == site_id_1);
    assert(index_result.has_value());
    assert(*index_result == index);

    // insert in tail
    index = 1;
    s = editor2.local_insert(index, text[index]);
    index_result = editor1.remote_insert(s);

    assert(s.value() == text[index]);
    assert(s.site_id() == site_id_2);
    assert(index_result.has_value());
    assert(*index_result == index);
    assert(editor1.to_string() == text);
    assert(editor2.to_string() == text);

    return 0;
}