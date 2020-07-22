/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class CursorMessage : public Message {
        QString document_, username_;
        Symbol symbol_;

        QJsonObject json() const override;
    public:
        CursorMessage(const QString& document, const QString& username, const Symbol& symbol);
        QString document() const;
        QString username() const;
        Symbol symbol() const;
    };
}
