/*
 * Author: Franco Ruggeri
 */

#include "OpenMessage.h"

namespace collaborative_text_editor {
    OpenMessage::OpenMessage(QString document) : Message(MessageType::open), document_(document) {}

    QString OpenMessage::document() const {
        return document_;
    }

    QJsonObject OpenMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        return json_;
    }
}
