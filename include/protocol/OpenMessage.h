/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class OpenMessage : public Message {
        QString document_;

        QJsonObject json() const override;
    public:
        OpenMessage(const QString& document);
        QString document() const;
    };
}
