/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>
#include <editor/protocol/Profile.h>

namespace editor {
    class SignupMessage : public Message {
        Profile profile_;
        QString password_;

        SignupMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        SignupMessage(const Profile& profile, const QString& password);
        bool operator==(const Message& other) const override;
        Profile& profile();
        QString& password();
    };
}