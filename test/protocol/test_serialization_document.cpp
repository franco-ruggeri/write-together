/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/DocumentMessage.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/DocumentData.h>
#include <editor/crdt/SharedEditor.h>
#include <editor/crdt/Symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>

int main() {
    const editor::Document document("test owner", "test name");
    const QString text_string("test text");
    const int site_id(editor::SharedEditor::start_site_id), site_counter(editor::SharedEditor::start_site_counter);
    const QString sharing_link("fnsc:test_uri");
    const QString username1("test username 1"), username2("test username 2");

    // text
    editor::SharedEditor editor(site_id, site_counter);
    for (int i=0; i<text_string.size(); i++)
        editor.local_insert(i, text_string[i]);
    QList<editor::Symbol> text = editor.text();

    // site_id_others
    const QHash<QString,int> site_id_others{
            {username1, 1},
            {username2, 2}
    };

    // profile_others
    const QHash<QString,editor::Profile> profile_others{
            {username1, editor::Profile(username1, "test name 1", "test surname 1", QImage{})},
            {username2, editor::Profile(username2, "test name 2", "test surname 2", QImage{})}
    };

    // cursors
    const QHash<QString,editor::Symbol> cursors{
            {username1, text[0]},
            {username2, text[1]}
    };

    // serialization
    editor::DocumentData document_data(text, editor.site_id(), editor.site_counter(), site_id_others, profile_others,
                                       cursors, sharing_link);
    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::DocumentMessage>::create(document, document_data);
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}