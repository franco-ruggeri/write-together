/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/SignupOkMessage.h>
#include <QtCore/QSharedPointer>

int main() {
    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::SignupOkMessage>::create();
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}