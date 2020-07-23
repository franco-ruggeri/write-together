/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <protocol/Message.h>

namespace collaborative_text_editor {
    class ProfileOkMessage : public Message {
        ProfileOkMessage(const QJsonObject& json_object);
        friend Message;

    public:
        ProfileOkMessage();
        bool operator==(const Message& other) const override;
    };
}
