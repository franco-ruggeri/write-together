/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class EraseMessage : public Message {
        Document document_;
        Symbol symbol_;

        EraseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        EraseMessage(const Document& document, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        Document& document();
        Symbol& symbol();
    };
}
