/*
 * Author: Franco Ruggeri
 */

#include "SignUpMessage.h"

namespace collaborative_text_editor {
    SignUpMessage::SignUpMessage(std::string username, std::string password) :
        Message(MessageType::sign_up), username_(username), password_(password) {}

    std::string SignUpMessage::username() const {
        return username_;
    }

    std::string SignUpMessage::password() const {
        return password_;
    }
}
