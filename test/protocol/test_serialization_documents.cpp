/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentsMessage.h>
#include <protocol/Document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc%2 == 0) {
        std::cerr << "usage: " << argv[0] << " [document_owner document_name ...]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    QList<Document> documents;
    for (int i=1; i<argc; i+=2)
        documents.push_back(Document(argv[i], argv[i+1]));

    // original message
    message1 = QSharedPointer<DocumentsMessage>::create(documents);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}