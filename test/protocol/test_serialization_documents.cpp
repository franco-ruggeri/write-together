/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentsMessage.h>
#include <memory>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::shared_ptr<Message> message1, message2;

    std::vector<QString> documents;
    for (int i=1; i<argc; i++)
        documents.push_back(argv[i]);

    // original message
    message1 = std::make_shared<DocumentsMessage>(documents);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}