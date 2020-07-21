/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class EraseMessage : public Message {
        QString document_;
        Symbol symbol_;

    public:
        EraseMessage(QString document, Symbol symbol);
        QString document() const;
        Symbol symbol() const;
        QJsonObject json() const override;
    };
}
