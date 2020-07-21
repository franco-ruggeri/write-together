/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class CreateMessage : public Message {
        QString document_;

    public:
        CreateMessage(QString document);
        QString document() const;
        QJsonObject json() const override;
    };
}
