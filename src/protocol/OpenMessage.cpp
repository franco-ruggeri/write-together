/*
 * Author: Franco Ruggeri
 */

#include "OpenMessage.h"

namespace collaborative_text_editor {
    OpenMessage::OpenMessage(std::string document) : Message(MessageType::open), document_(document) {}

    std::string OpenMessage::document() const {
        return document_;
    }
}
