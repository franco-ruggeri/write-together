/*
 * Author: Franco Ruggeri
 */

#include "LoginMessage.h"

namespace collaborative_text_editor {
    LoginMessage::LoginMessage(const QString& username, const QString& password) :
        Message(MessageType::login), username_(username), password_(password) {}

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
