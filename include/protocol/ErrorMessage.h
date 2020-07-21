/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class ErrorMessage : public Message {
        QString reason_;

        QJsonObject json() const override;
    public:
        ErrorMessage(QString reason);
        QString reason() const;
    };
}
