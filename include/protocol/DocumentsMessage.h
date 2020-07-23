/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>
#include <QString>
#include <QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        std::vector<QString> documents_;

        DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage(const std::vector<QString>& documents);
        bool operator==(const Message& other) const override;
        std::vector<QString> documents() const;
    };
}
