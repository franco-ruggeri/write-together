/*
 * Author: Franco Ruggeri
 */

#include <protocol/CursorMessage.h>
#include <protocol/Document.h>
#include <crdt/Symbol.h>
#include <QtCore/QSharedPointer>

using namespace collaborative_text_editor;

int main() {
    const Document document("test owner", "test name");
    const QString username("test username");
    const Symbol symbol('C', 1, 5, {2, 4, 5});

    QSharedPointer<Message> message1 = QSharedPointer<CursorMessage>::create(document, username, symbol);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}