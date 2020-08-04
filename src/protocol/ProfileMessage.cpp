/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileMessage.h>

namespace editor {
    ProfileMessage::ProfileMessage(const Profile& profile) :
            Message(MessageType::profile), profile_(profile) {}

    ProfileMessage::ProfileMessage(const Profile& profile, const QString& password) :
        Message(MessageType::profile), profile_(profile), password_(password) {}

    ProfileMessage::ProfileMessage(const QJsonObject &json_object) : Message(MessageType::profile) {
        auto end_iterator = json_object.end();
        auto profile_iterator = json_object.find("profile");
        auto password_iterator = json_object.find("password");

        if (profile_iterator == end_iterator || !profile_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        profile_ = Profile(profile_iterator->toObject());
        if (password_iterator != end_iterator) {
            password_ = password_iterator->toString();
            if (password_->isNull())
                throw std::logic_error("invalid message: invalid fields");
        }
    }

    bool ProfileMessage::operator==(const Message& other) const {
        const ProfileMessage *o = dynamic_cast<const ProfileMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->profile_ == o->profile_ && this->password_ == o->password_;
    }

    Profile ProfileMessage::profile() const {
        return profile_;
    }

    std::optional<QString> ProfileMessage::password() const {
        return password_;
    }

    QJsonObject ProfileMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["profile"] = profile_.json();
        if (password_) json_object["password"] = *password_;
        return json_object;
    }
}
