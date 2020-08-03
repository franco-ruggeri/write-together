/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <editor/protocol/Message.h>

namespace editor {
    class ProfileOkMessage : public Message {
        ProfileOkMessage(const QJsonObject& json_object);
        friend Message;

    public:
        ProfileOkMessage();
        bool operator==(const Message& other) const override;
    };
}
