/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/DocumentsMessage.h>
#include <editor/protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QSet>

int main() {
    const QSet<editor::Document> documents = {
            editor::Document("test owner 1", "test name 1"),
            editor::Document("test owner 2", "test name 2")
    };

    QSharedPointer<editor::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<editor::DocumentsMessage>::create(documents);
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<editor::DocumentsMessage>::create();
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}