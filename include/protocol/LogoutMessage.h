/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace editor {
    class LogoutMessage : public Message {
        LogoutMessage(const QJsonObject& json_object);
        friend Message;

    public:
        LogoutMessage();
        bool operator==(const Message& other) const override;
    };
}
