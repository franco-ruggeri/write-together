/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class CreateMessage : public Message {
        QString document_name_;

        CreateMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CreateMessage(const QString& document_name);
        bool operator==(const Message& other) const override;
        QString& document_name();
    };
}
