/*
 * Author: Franco Ruggeri
 */

#include <protocol/EraseMessage.h>
#include <protocol/Document.h>
#include <crdt/Symbol.h>
#include <QtCore/QSharedPointer>

using namespace collaborative_text_editor;

int main() {
    const Document document("test owner", "test name");
    const Symbol symbol('C', 1, 5, {2, 5, 1});

    QSharedPointer<Message> message1 = QSharedPointer<EraseMessage>::create(document, symbol);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}