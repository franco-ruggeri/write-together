/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>
#include <protocol/Document.h>
#include <crdt/SharedEditor.h>
#include <crdt/Symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>

using namespace collaborative_text_editor;

int main() {
    const Document document("test owner", "test name");
    const QString text_string("test text");
    const int site_id(SharedEditor::start_site_id), site_counter(SharedEditor::start_site_counter);
    const QString sharing_link("fnsc:test_uri");
    const QString username1("test username 1"), username2("test username 2");

    // text
    SharedEditor editor(site_id, site_counter);
    for (int i=0; i<text_string.size(); i++)
        editor.local_insert(i, text_string[i]);
    QList<Symbol> text = editor.text();

    // site_ids
    const QHash<QString,int> site_ids{
            {username1, 1},
            {username2, 2}
    };

    // profiles
    const QHash<QString,Profile> profiles{
            {username1, Profile(username1, "test name 1", "test surname 1")},
            {username2, Profile(username2, "test name 2", "test surname 2")}
    };

    // cursors
    const QHash<QString,Symbol> cursors{
            {username1, text[0]},
            {username2, text[1]}
    };

    // serialization
    QSharedPointer<Message> message1 = QSharedPointer<DocumentMessage>::create(document, text, editor.site_id(),
                                                                               editor.site_counter(), site_ids,
                                                                               profiles, cursors, sharing_link);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}