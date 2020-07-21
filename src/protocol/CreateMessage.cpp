/*
 * Author: Franco Ruggeri
 */

#include "CreateMessage.h"

namespace collaborative_text_editor {
    CreateMessage::CreateMessage(std::string document) : Message(MessageType::create), document_(document) {}

    std::string CreateMessage::document() const {
        return document_;
    }
}