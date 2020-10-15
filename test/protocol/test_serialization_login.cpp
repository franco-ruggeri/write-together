/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/login_message.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <assert.h>

int main() {
    const QString username("test username");
    const QString password("test password");

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::LoginMessage>::create(username, password);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}