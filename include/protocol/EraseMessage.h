/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include <protocol/Message.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class EraseMessage : public Message {
        QString document_;
        Symbol symbol_;

        EraseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        EraseMessage(const QString& document, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        QString document() const;
        Symbol symbol() const;
    };
}
