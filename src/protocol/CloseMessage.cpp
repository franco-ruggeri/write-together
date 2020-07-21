/*
 * Author: Franco Ruggeri
 */

#include "CloseMessage.h"

namespace collaborative_text_editor {
    CloseMessage::CloseMessage(std::string document, std::string username) :
        Message(MessageType::close), document_(document), username_(username) {}

    std::string CloseMessage::document() const {
        return document_;
    }

    std::string CloseMessage::username() const {
        return username_;
    }
}
