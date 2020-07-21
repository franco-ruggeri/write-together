/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class TextMessage : public Message {
        QString document_, text_;

    public:
        TextMessage(QString document, QString text);
        QString document() const;
        QString text() const;
        QJsonObject json() const override;
    };
}
