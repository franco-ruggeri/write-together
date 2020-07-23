/*
 * Author: Franco Ruggeri
 */

#include <protocol/SignUpMessage.h>

namespace collaborative_text_editor {
    SignUpMessage::SignUpMessage(const QString& username, const QString& password) :
        Message(MessageType::sign_up), username_(username), password_(password) {}

    SignUpMessage::SignUpMessage(const QJsonObject &json_object) : Message(MessageType::sign_up) {
        auto end_iterator = json_object.end();
        auto username_iterator = json_object.find("username");
        auto password_iterator = json_object.find("password");

        if (username_iterator == end_iterator || password_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        username_ = username_iterator->toString();
        password_ = password_iterator->toString();

        if (username_.isNull() || password_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool SignUpMessage::operator==(const Message& other) const {
        const SignUpMessage *o = dynamic_cast<const SignUpMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->username_ == o->username_ && this->password_ == o->password_;
    }
        
    QString SignUpMessage::username() const {
        return username_;
    }

    QString SignUpMessage::password() const {
        return password_;
    }

    QJsonObject SignUpMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["username"] = username_;
        json_object["password"] = password_;
        return json_object;
    }
}
