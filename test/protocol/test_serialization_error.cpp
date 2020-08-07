/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/ErrorMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const QString reason("test reason");

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::ErrorMessage>::create(reason);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}