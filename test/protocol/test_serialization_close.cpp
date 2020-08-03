/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/CloseMessage.h>
#include <editor/protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

int main() {
    const editor::Document document("test owner", "test name");
    const QString username("test username");

    QSharedPointer<editor::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<editor::CloseMessage>::create(document, username);
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<editor::CloseMessage>::create(document);
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}