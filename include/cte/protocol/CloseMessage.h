/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <optional>

namespace cte {
    class CloseMessage : public Message {
        Document document_;
        std::optional<int> site_id_;

        explicit CloseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        explicit CloseMessage(const Document& document);
        CloseMessage(const Document& document, int site_id);
        bool operator==(const Message& other) const override;
        Document document() const;
        std::optional<int> site_id() const;
    };
}