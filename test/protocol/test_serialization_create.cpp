/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/CreateMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

using namespace editor;

int main() {
    const QString document_name("test name");

    QSharedPointer<Message> message1 = QSharedPointer<CreateMessage>::create(document_name);
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}