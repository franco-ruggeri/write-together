/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class InsertMessage : public Message {
        QString document_;
        Symbol symbol_;

        QJsonObject json() const override;
    public:
        InsertMessage(QString document, Symbol symbol);
        QString document() const;
        Symbol symbol() const;
    };
}
