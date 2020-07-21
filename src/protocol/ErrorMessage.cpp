/*
 * Author: Franco Ruggeri
 */

#include "ErrorMessage.h"

namespace collaborative_text_editor {
    ErrorMessage::ErrorMessage(QString reason) : Message(MessageType::error), reason_(reason) {}

    QString ErrorMessage::reason() const {
        return reason_;
    }

    QJsonObject ErrorMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("reason", reason_);
        return json_;
    }
}
