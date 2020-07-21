/*
 * Author: Franco Ruggeri
 */

#include "SignUpMessage.h"

namespace collaborative_text_editor {
    SignUpMessage::SignUpMessage(QString username, QString password) :
        Message(MessageType::sign_up), username_(username), password_(password) {}

    QString SignUpMessage::username() const {
        return username_;
    }

    QString SignUpMessage::password() const {
        return password_;
    }

    QJsonObject SignUpMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("username", username_);
        json_.insert("password", password_);
        return json_;
    }
}
