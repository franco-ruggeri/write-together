/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/CreateMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const QString document_name("test name");

    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::CreateMessage>::create(document_name);
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}