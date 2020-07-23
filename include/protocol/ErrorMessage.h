/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class ErrorMessage : public Message {
        QString reason_;

        ErrorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        ErrorMessage(const QString& reason);
        bool operator==(const Message& other) const override;
        QString reason() const;
    };
}
