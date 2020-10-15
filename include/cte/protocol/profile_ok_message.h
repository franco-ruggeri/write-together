/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <cte/protocol/message.h>

namespace cte {
    class ProfileOkMessage : public Message {
        explicit ProfileOkMessage(const QJsonObject& json_object);
        friend Message;

    public:
        ProfileOkMessage();
        bool operator==(const Message& other) const override;
    };
}
