/*
 * Author: Franco Ruggeri
 */

#include "LoginMessage.h"

namespace collaborative_text_editor {
    LoginMessage::LoginMessage(QString username, QString password)
        : Message(MessageType::login), username_(username), password_(password) {}

    QString LoginMessage::username() const {
        return username_;
    }

    QString LoginMessage::password() const {
        return password_;
    }

    QJsonObject LoginMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("username", username_);
        json_.insert("password", password_);
        return json_;
    }
}
