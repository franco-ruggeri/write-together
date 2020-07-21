/*
 * Author: Franco Ruggeri
 */

#include "TextMessage.h"

namespace collaborative_text_editor {
    TextMessage::TextMessage(std::string document, std::string text) :
        Message(MessageType::text), document_(document), text_(text) {}

    std::string TextMessage::document() const {
        return document_;
    }

    std::string TextMessage::text() const {
        return text_;
    }
}
