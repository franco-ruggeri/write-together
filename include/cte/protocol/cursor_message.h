/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>
#include <optional>

namespace cte {
    class CursorMessage : public Message {
        Document document_;
        Symbol symbol_;
        std::optional<int> site_id_;

        explicit CursorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CursorMessage(const Document& document, const Symbol& symbol);
        CursorMessage(const Document& document, const Symbol& symbol, int site_id);
        bool operator==(const Message& other) const override;
        Document document() const;
        Symbol symbol() const;
        std::optional<int> site_id() const;
    };
}