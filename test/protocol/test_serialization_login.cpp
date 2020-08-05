/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/LoginMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const QString username("test username");
    const QString password("test password");

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::LoginMessage>::create(username, password);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}