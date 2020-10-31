#include <cte/protocol/cursor_message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QSharedPointer>
#include <cassert>

int main() {
    const cte::Document document("test owner", "test name");
    const int site_id = 1;
    const cte::Symbol symbol('C', 1, 5, {2, 4, 5});

    QSharedPointer<cte::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<cte::CursorMessage>::create(document, symbol, site_id);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<cte::CursorMessage>::create(document, symbol);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}