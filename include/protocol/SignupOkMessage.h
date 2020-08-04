/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <protocol/Message.h>

namespace editor {
    class SignupOkMessage : public Message {
        SignupOkMessage(const QJsonObject& json_object);
        friend Message;

    public:
        SignupOkMessage();
        bool operator==(const Message& other) const override;
    };
}
