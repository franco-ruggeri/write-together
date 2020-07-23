/*
 * Author: Franco Ruggeri
 */

#include <protocol/LoginMessage.h>

namespace collaborative_text_editor {
    LoginMessage::LoginMessage(const QString& username, const QString& password) :
        Message(MessageType::login), username_(username), password_(password) {}

    LoginMessage::LoginMessage(const QJsonObject &json_object) : Message(MessageType::login) {
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

    bool LoginMessage::operator==(const Message& other) const {
        const LoginMessage *o = dynamic_cast<const LoginMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->username_ == o->username_ && this->password_ == o->password_;
    }
        
    QString LoginMessage::username() const {
        return username_;
    }

    QString LoginMessage::password() const {
        return password_;
    }

    QJsonObject LoginMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["username"] = username_;
        json_object["password"] = password_;
        return json_object;
    }
}
