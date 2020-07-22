/*
 * Author: Franco Ruggeri
 */

#include "CloseMessage.h"

namespace collaborative_text_editor {
    CloseMessage::CloseMessage(const QString& document, const QString& username) :
        Message(MessageType::close), document_(document), username_(username) {}

    QString CloseMessage::document() const {
        return document_;
    }

    QString CloseMessage::username() const {
        return username_;
    }

    QJsonObject CloseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["username"] = username_;
        return json_object;
    }
}
