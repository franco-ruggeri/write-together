/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileOkMessage.h>
#include <QtCore/QSharedPointer>

using namespace collaborative_text_editor;

int main() {
    QSharedPointer<Message> message1 = QSharedPointer<ProfileOkMessage>::create();
    QSharedPointer<Message> message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}