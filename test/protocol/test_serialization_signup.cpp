/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/SignupMessage.h>
#include <editor/protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const editor::Profile username("test username", "test name", "test surname", QImage{});
    const QString password("test password");

    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::SignupMessage>::create(username, password);
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}