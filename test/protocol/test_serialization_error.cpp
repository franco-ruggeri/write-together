/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/ErrorMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const QString reason("test reason");

    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::ErrorMessage>::create(reason);
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}