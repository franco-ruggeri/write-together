/*
 * Author: Franco Ruggeri
 */

#include <protocol/LogoutMessage.h>

namespace editor {
    LogoutMessage::LogoutMessage() : Message(MessageType::logout) {}

    LogoutMessage::LogoutMessage(const QJsonObject &json_object) : Message(MessageType::logout) {}

    bool LogoutMessage::operator==(const Message& other) const {
        const LogoutMessage *o = dynamic_cast<const LogoutMessage*>(&other);
        return o != nullptr && this->type() == o->type();
    }
}
