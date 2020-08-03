/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/Profile.h>

namespace editor {
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
        std::optional<Document>& document();
        std::optional<QString>& sharing_link();
        std::optional<int>& site_id();
        std::optional<Profile>& profile();
    };
}