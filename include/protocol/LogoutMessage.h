/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class LogoutMessage : public Message {
        QJsonObject json() const override;
    public:
        LogoutMessage();
    };
}
