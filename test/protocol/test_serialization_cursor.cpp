/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/CursorMessage.h>
#include <cte/protocol/Document.h>
#include <cte/crdt/Symbol.h>
#include <QtCore/QSharedPointer>

int main() {
    const cte::Document document("test owner", "test name");
    const QString username("test username");
    const cte::Symbol symbol('C', 1, 5, {2, 4, 5});

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::CursorMessage>::create(document, symbol, username);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}