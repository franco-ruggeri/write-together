/*
 * Author: Franco Ruggeri
 */

#include <protocol/SignupMessage.h>

namespace collaborative_text_editor {
    SignupMessage::SignupMessage(const QString& username, const QString& password) :
            Message(MessageType::signup), username_(username), password_(password) {}

    SignupMessage::SignupMessage(const QJsonObject &json_object) : Message(MessageType::signup) {
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

    bool SignupMessage::operator==(const Message& other) const {
        const SignupMessage *o = dynamic_cast<const SignupMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->username_ == o->username_ && this->password_ == o->password_;
    }
        
    QString& SignupMessage::username() {
        return username_;
    }

    QString& SignupMessage::password() {
        return password_;
    }

    QJsonObject SignupMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["username"] = username_;
        json_object["password"] = password_;
        return json_object;
    }
}
