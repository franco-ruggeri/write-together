/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/InsertMessage.h>
#include <editor/protocol/Document.h>
#include <editor/crdt/Symbol.h>
#include <QtCore/QSharedPointer>

int main() {
    const editor::Document document("test owner", "test name");
    const editor::Symbol symbol('C', 1, 5, {2, 5, 1});

    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::InsertMessage>::create(document, symbol);
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}