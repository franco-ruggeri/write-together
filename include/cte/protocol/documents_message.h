#pragma once

#include <QtCore/QList>
#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <optional>

namespace cte {
    class DocumentsMessage : public Message {
        std::optional<QList<Document>> documents_;

        explicit DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage();
        explicit DocumentsMessage(const QList<Document>& documents);
        bool operator==(const Message& other) const override;
        std::optional<QList<Document>> documents() const;
    };
}
