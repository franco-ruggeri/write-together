/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class InsertMessage : public Message {
        QString document_;
        Symbol symbol_;

    public:
        InsertMessage(QString document, Symbol symbol);
        QString document() const;
        Symbol symbol() const;
        QJsonObject json() const override;
    };
}
