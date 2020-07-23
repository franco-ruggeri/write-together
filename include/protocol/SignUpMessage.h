/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QString>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class SignUpMessage : public Message {
        QString username_, password_;

        SignUpMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        SignUpMessage(const QString& username, const QString& password);
        bool operator==(const Message& other) const override;
        QString username() const;
        QString password() const;
    };
}
