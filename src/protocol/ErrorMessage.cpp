/*
 * Author: Franco Ruggeri
 */

#include "ErrorMessage.h"

namespace collaborative_text_editor {
    ErrorMessage::ErrorMessage(const QString& reason) : Message(MessageType::error), reason_(reason) {}

    QString ErrorMessage::reason() const {
        return reason_;
    }

    QJsonObject ErrorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["reason"] = reason_;
        return json_object;
    }
}
