/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/SignupMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

using namespace editor;

int main() {
    const QString username("test username");
    const QString password("test password");

    QSharedPointer<Message> message1 = QSharedPointer<SignupMessage>::create(username, password);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}