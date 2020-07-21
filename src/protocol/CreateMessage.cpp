/*
 * Author: Franco Ruggeri
 */

#include "CreateMessage.h"

namespace collaborative_text_editor {
    CreateMessage::CreateMessage(QString document) : Message(MessageType::create), document_(document) {}

    QString CreateMessage::document() const {
        return document_;
    }

    QJsonObject CreateMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        return json_;
    }
}