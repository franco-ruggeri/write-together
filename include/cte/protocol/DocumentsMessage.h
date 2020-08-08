/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSet>
#include <QtCore/QJsonObject>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>

namespace cte {
    class DocumentsMessage : public Message {
        std::optional<QSet<Document>> documents_;

        explicit DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage();
        explicit DocumentsMessage(const QSet<Document>& documents);
        bool operator==(const Message& other) const override;
        std::optional<QSet<Document>> documents() const;
    };
}
