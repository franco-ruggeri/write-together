/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/ProfileOkMessage.h>
#include <QtCore/QSharedPointer>

int main() {
    QSharedPointer<editor::Message> message1 = QSharedPointer<editor::ProfileOkMessage>::create();
    QSharedPointer<editor::Message> message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}