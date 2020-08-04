/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSet>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>

namespace editor {
    class DocumentsMessage : public Message {
        std::optional<QSet<Document>> documents_;

        DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage();
        DocumentsMessage(const QSet<Document>& documents);
        bool operator==(const Message& other) const override;
        std::optional<QSet<Document>> documents() const;
    };
}
