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
        std::optional<QString> username_;

        explicit CloseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        explicit CloseMessage(const Document& document);
        CloseMessage(const Document& document, const QString& username);
        bool operator==(const Message& other) const override;
        Document document() const;
        std::optional<QString> username() const;
    };
}