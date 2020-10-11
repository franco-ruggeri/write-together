/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/signup_message.h>

namespace cte {
    SignupMessage::SignupMessage(const Profile& profile, const QString& password) :
            Message(MessageType::signup), profile_(profile), password_(password) {}

    SignupMessage::SignupMessage(const QJsonObject &json_object) : Message(MessageType::signup) {
        auto end_iterator = json_object.end();
        auto profile_iterator = json_object.find("profile");
        auto password_iterator = json_object.find("password");

        if (profile_iterator == end_iterator || password_iterator == end_iterator || !profile_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        profile_ = Profile(profile_iterator->toObject());
        password_ = password_iterator->toString();

        if (password_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool SignupMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const SignupMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->profile_ == o->profile_ && this->password_ == o->password_;
    }
        
    Profile SignupMessage::profile() const {
        return profile_;
    }

    QString SignupMessage::password() const {
        return password_;
    }

    QJsonObject SignupMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["profile"] = profile_.json();
        json_object["password"] = password_;
        return json_object;
    }
}
