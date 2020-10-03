/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/SignupMessage.h>
#include <cte/protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <assert.h>

int main() {
    const cte::Profile username("test username", "test name", "test surname", QImage{});
    const QString password("test password");

    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::SignupMessage>::create(username, password);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}