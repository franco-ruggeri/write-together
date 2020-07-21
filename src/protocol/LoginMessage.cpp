/*
 * Author: Franco Ruggeri
 */

#include "LoginMessage.h"

namespace collaborative_text_editor {
    LoginMessage::LoginMessage(std::string username, std::string password)
        : Message(MessageType::login), username_(username), password_(password) {}

    std::string LoginMessage::username() const {
        return username_;
    }

    std::string LoginMessage::password() const {
        return password_;
    }
}
