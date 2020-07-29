/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        QList<QString> documents_;

        DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage(const QList<QString>& documents);
        bool operator==(const Message& other) const override;
        QList<QString> documents() const;
    };
}
