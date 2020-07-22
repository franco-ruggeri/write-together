/*
 * Author: Franco Ruggeri
 */

#include "CreateMessage.h"

namespace collaborative_text_editor {
    CreateMessage::CreateMessage(const QString& document) : Message(MessageType::create), document_(document) {}

    QString CreateMessage::document() const {
        return document_;
    }

    QJsonObject CreateMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        return json_object;
    }
}