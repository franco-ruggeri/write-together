/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>
#include <protocol/Document.h>
#include <crdt/SharedEditor.h>
#include <crdt/Symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QString>

using namespace collaborative_text_editor;

int main() {
    const Document document("test owner", "test name");
    const QString text("test text");
    const QString sharing_link("fnsc:test_uri");

    // users
    const QHash<int,Profile> users{
            {1, Profile("test username 1", "test name 1", "test surname 1")},
            {2, Profile("test username 2", "test name 2", "test surname 2")}
    };

    // text
    SharedEditor editor(0);
    for (int i=0; i<text.size(); i++)
        editor.local_insert(i, text[i]);
    QVector<Symbol> symbols = editor.text();

    // cursors
    const QHash<QString,Symbol> cursors{
            {users[1].username(), symbols[0]},
            {users[2].username(), symbols[1]}
    };

    // serialization
    QSharedPointer<Message> message1 = QSharedPointer<DocumentMessage>::create(document, symbols, users, cursors,
                                                                               sharing_link);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}