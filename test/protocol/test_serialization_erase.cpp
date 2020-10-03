/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/EraseMessage.h>
#include <cte/protocol/Document.h>
#include <cte/crdt/Symbol.h>
#include <QtCore/QSharedPointer>
#include <assert.h>

int main() {
    const cte::Document document("test owner", "test name");
    const cte::Symbol symbol('C', 1, 5, {2, 5, 1});

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::EraseMessage>::create(document, symbol);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}