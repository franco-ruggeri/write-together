/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class CursorMessage : public Message {
        QString document_, username_;
        Symbol symbol_;

    public:
        CursorMessage(QString document, QString username, Symbol symbol);
        QString document() const;
        QString username() const;
        Symbol symbol() const;
        QJsonObject json() const override;
    };
}
