/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>

namespace editor {
    class LoginMessage : public Message {
        QString username_, password_;

        LoginMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        LoginMessage(const QString& username, const QString& password);
        bool operator==(const Message& other) const override;
        QString username() const;
        QString password() const;
    };
}
