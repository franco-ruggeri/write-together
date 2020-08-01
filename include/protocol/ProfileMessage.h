/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtGui/QImage>
#include <protocol/Message.h>
#include <protocol/Profile.h>

namespace collaborative_text_editor {
    class ProfileMessage : public Message {
        Profile profile_;
        std::optional<QString> password_;

        ProfileMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        ProfileMessage(const Profile& profile);
        ProfileMessage(const Profile& profile, const QString& password);
        bool operator==(const Message& other) const override;
        Profile& profile();
        std::optional<QString>& password();
    };
}