/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class TextMessage : public Message {
        QString document_, text_;

        QJsonObject json() const override;
    public:
        TextMessage(const QString& document, const QString& text);
        QString document() const;
        QString text() const;
    };
}
