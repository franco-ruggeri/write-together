/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QString>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class SignupMessage : public Message {
        QString username_, password_;

        SignupMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        SignupMessage(const QString& username, const QString& password);
        bool operator==(const Message& other) const override;
        QString username() const;
        QString password() const;
    };
}
