#pragma once

#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class EraseMessage : public Message {
        Document document_;
        Symbol symbol_;
        std::optional<int> site_id_;

        explicit EraseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        EraseMessage(const Document& document, const Symbol& symbol);
        EraseMessage(const Document& document, const Symbol& symbol, int site_id);
        bool operator==(const Message& other) const override;
        Document document() const;
        Symbol symbol() const;
        std::optional<int> site_id() const;
    };
}
