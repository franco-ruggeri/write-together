/*
 * Author: Franco Ruggeri
 */

#include "Message.h"

namespace collaborative_text_editor {
    Message::Message(MessageType type) : type_(type) {}

    MessageType Message::type() const {
        return type_;
    }

    QJsonObject Message::json() const {
        return {{"type", static_cast<int>(type_)}};
    }
}
