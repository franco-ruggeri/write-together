/*
 * Author: Franco Ruggeri
 */

#include "Message.h"
#include <QJsonDocument>

namespace collaborative_text_editor {
    Message::Message(MessageType type) : type_(type) {}

    MessageType Message::type() const {
        return type_;
    }

    QJsonObject Message::json() const {
        return {{"type", static_cast<int>(type_)}};
    }

    QByteArray Message::serialize() const {
        QJsonObject json_object = json();
        QJsonDocument json_document(json_object);
        QByteArray json_bytes = json_document.toJson(QJsonDocument::JsonFormat::Compact);
        return json_bytes;
    }
}
