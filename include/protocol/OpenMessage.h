/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <protocol/Profile.h>
#include <optional>

namespace collaborative_text_editor {
    class OpenMessage : public Message {
        std::optional<Document> document_;
        std::optional<QString> sharing_link_;
        std::optional<int> site_id_;
        std::optional<Profile> profile_;

        OpenMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        OpenMessage(const Document& document);
        OpenMessage(const Document& document, int site_id, const Profile& profile);
        OpenMessage(const QString& sharing_link);
        bool operator==(const Message& other) const override;
        std::optional<Document> document() const;
        std::optional<QString> sharing_link() const;
        std::optional<int> site_id() const;
        std::optional<Profile> profile() const;
    };
}
