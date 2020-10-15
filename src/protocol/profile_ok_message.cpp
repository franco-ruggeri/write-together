/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/profile_ok_message.h>

namespace cte {
    ProfileOkMessage::ProfileOkMessage() : Message(MessageType::profile_ok) {}

    ProfileOkMessage::ProfileOkMessage(const QJsonObject &json_object) : Message(MessageType::profile_ok) {}

    bool ProfileOkMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const ProfileOkMessage*>(&other);
        return o != nullptr && this->type() == o->type();
    }
}
