#include <cte/protocol/insert_message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QSharedPointer>
#include <cassert>

int main() {
    const cte::Document document("test owner", "test name");
    const cte::Symbol symbol('C', 1, 5, {2, 5, 1});

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::InsertMessage>::create(document, symbol);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}