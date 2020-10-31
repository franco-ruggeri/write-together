#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>
#include <cte/protocol/message.h>
#include <cte/protocol/profile.h>
#include <optional>

namespace cte {
    class ProfileMessage : public Message {
        Profile profile_;
        std::optional<QString> password_;

        explicit ProfileMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        explicit ProfileMessage(const Profile& profile);
        ProfileMessage(const Profile& profile, const QString& password);
        bool operator==(const Message& other) const override;
        Profile profile() const;
        std::optional<QString> password() const;
    };
}