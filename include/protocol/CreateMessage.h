/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class CreateMessage : public Message {
        QString document_;

        QJsonObject json() const override;
    public:
        CreateMessage(const QString& document);
        QString document() const;
    };
}
