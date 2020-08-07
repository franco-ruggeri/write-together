/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <cte/crdt/Symbol.h>

namespace cte {
    class EraseMessage : public Message {
        Document document_;
        Symbol symbol_;

        explicit EraseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        EraseMessage(const Document& document, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        Document document() const;
        Symbol symbol() const;
    };
}