/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class CreateMessage : public Message {
        QString document_;

        CreateMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CreateMessage(const QString& document);
        bool operator==(const Message& other) const override;
        QString document() const;
    };
}
