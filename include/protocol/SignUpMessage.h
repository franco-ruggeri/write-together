/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class SignUpMessage : public Message {
        QString username_, password_;

    public:
        SignUpMessage(QString username, QString password);
        QString username() const;
        QString password() const;
        QJsonObject json() const override;
    };
}
