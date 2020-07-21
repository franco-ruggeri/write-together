/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class ErrorMessage : public Message {
        QString reason_;

    public:
        ErrorMessage(QString reason);
        QString reason() const;
        QJsonObject json() const override;
    };
}
