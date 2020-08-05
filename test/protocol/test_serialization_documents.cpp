/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/DocumentsMessage.h>
#include <editor/protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QSet>

int main() {
    const QSet<cte::Document> documents = {
            cte::Document("test owner 1", "test name 1"),
            cte::Document("test owner 2", "test name 2")
    };

    QSharedPointer<cte::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<cte::DocumentsMessage>::create(documents);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<cte::DocumentsMessage>::create();
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}