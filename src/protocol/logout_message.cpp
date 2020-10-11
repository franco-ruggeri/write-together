/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/logout_message.h>

namespace cte {
    LogoutMessage::LogoutMessage() : Message(MessageType::logout) {}

    LogoutMessage::LogoutMessage(const QJsonObject &json_object) : Message(MessageType::logout) {}

    bool LogoutMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const LogoutMessage*>(&other);
        return o != nullptr && this->type() == o->type();
    }
}
