/*
 * Author: Franco Ruggeri
 */

#include <protocol/LogoutMessage.h>
#include <memory>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    std::shared_ptr<Message> message1, message2;

    // original message
    message1 = std::make_shared<LogoutMessage>();

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}