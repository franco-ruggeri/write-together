/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentsMessage.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    QVector<QString> documents;
    for (int i=1; i<argc; i++)
        documents.push_back(argv[i]);

    // original message
    message1 = QSharedPointer<DocumentsMessage>::create(documents);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}