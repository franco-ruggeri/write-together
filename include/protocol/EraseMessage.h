/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class EraseMessage : public Message {
        QString document_;
        Symbol symbol_;

        QJsonObject json() const override;
    public:
        EraseMessage(const QString& document, const Symbol& symbol);
        QString document() const;
        Symbol symbol() const;
    };
}
