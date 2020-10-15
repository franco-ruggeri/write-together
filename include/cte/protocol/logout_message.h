/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>

namespace cte {
    class LogoutMessage : public Message {
        explicit LogoutMessage(const QJsonObject& json_object);
        friend Message;

    public:
        LogoutMessage();
        bool operator==(const Message& other) const override;
    };
}
