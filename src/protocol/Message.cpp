/*
 * Author: Franco Ruggeri
 */

#include "Message.h"

namespace collaborative_text_editor {
    Message::Message(MessageType type) : type_(type) {}

    MessageType Message::type() const {
        return type_;
    }
}
