/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <optional>

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        std::optional<QList<Document>> documents_;

        DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage();
        DocumentsMessage(const QList<Document>& documents);
        bool operator==(const Message& other) const override;
        std::optional<QList<Document>> documents() const;
    };
}
