#pragma once

#include <cte/protocol/message.h>

namespace cte {
    class SignupOkMessage : public Message {
        explicit SignupOkMessage(const QJsonObject& json_object);
        friend Message;

    public:
        SignupOkMessage();
        bool operator==(const Message& other) const override;
    };
}
