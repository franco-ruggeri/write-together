/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QImage>
#include <QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class ProfileMessage : public Message {
        QString username_, password_;
        QImage icon_;

        ProfileMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        ProfileMessage(const QString& username, const QString& password, const QImage& icon);
        bool operator==(const Message& other) const override;
        QString username() const;
        QString password() const;
        QImage icon() const;
    };
}