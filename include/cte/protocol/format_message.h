#pragma once

#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/format.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class FormatMessage : public Message {
        Document document_;
        Symbol symbol_;
        Format format_;

        explicit FormatMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        FormatMessage(const Document& document, const Symbol& symbol, const Format& format);
        bool operator==(const Message& other) const override;
        Document document() const;
        Symbol symbol() const;
        Format format() const;
    };
}
