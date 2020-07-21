/*
 * Author: Franco Ruggeri
 */

#include "ErrorMessage.h"

namespace collaborative_text_editor {
    ErrorMessage::ErrorMessage(std::string reason) : Message(MessageType::error), reason_(reason) {}

    std::string ErrorMessage::reason() const {
        return reason_;
    }
}
