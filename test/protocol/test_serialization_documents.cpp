/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentsMessage.h>
#include <protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>

using namespace collaborative_text_editor;

int main() {
    const QList<Document> documents = {
            Document("test owner 1", "test name 1"),
            Document("test owner 2", "test name 2")
    };

    QSharedPointer<Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<DocumentsMessage>::create(documents);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<DocumentsMessage>::create();
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}