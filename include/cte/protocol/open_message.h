/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/profile.h>
#include <optional>

namespace cte {
    class OpenMessage : public Message {
        std::optional<Document> document_;
        std::optional<QString> sharing_link_;
        std::optional<int> site_id_;
        std::optional<Profile> profile_;

        explicit OpenMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        explicit OpenMessage(const Document& document);
        explicit OpenMessage(const QString& sharing_link);
        OpenMessage(const Document& document, int site_id, const Profile& profile);

        bool operator==(const Message& other) const override;

        std::optional<Document> document() const;
        std::optional<QString> sharing_link() const;
        std::optional<int> site_id() const;
        std::optional<Profile> profile() const;
    };
}
