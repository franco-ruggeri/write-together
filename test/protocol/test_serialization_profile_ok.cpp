/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileOkMessage.h>
#include <QtCore/QSharedPointer>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    // original message
    message1 = QSharedPointer<ProfileOkMessage>::create();

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}