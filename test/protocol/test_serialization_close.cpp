/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/CloseMessage.h>
#include <cte/protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const cte::Document document("test owner", "test name");
    const QString username("test username");

    QSharedPointer<cte::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<cte::CloseMessage>::create(document, username);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<cte::CloseMessage>::create(document);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}