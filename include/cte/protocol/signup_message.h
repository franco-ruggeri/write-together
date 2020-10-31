#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/profile.h>

namespace cte {
    class SignupMessage : public Message {
        Profile profile_;
        QString password_;

        explicit SignupMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        SignupMessage(const Profile& profile, const QString& password);
        bool operator==(const Message& other) const override;
        Profile profile() const;
        QString password() const;
    };
}
