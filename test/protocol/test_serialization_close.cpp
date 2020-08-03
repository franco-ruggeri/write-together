/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/CloseMessage.h>
#include <editor/protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

using namespace editor;

int main() {
    const Document document("test owner", "test name");
    const QString username("test username");

    QSharedPointer<Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<CloseMessage>::create(document, username);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<CloseMessage>::create(document);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}