/*
 * Author: Franco Ruggeri
 */

#include "TextMessage.h"

namespace collaborative_text_editor {
    TextMessage::TextMessage(QString document, QString text) :
        Message(MessageType::text), document_(document), text_(text) {}

    QString TextMessage::document() const {
        return document_;
    }

    QString TextMessage::text() const {
        return text_;
    }

    QJsonObject TextMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        json_.insert("text", text_);
        return json_;
    }
}
