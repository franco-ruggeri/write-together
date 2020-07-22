/*
 * Author: Franco Ruggeri
 */

#include "TextMessage.h"

namespace collaborative_text_editor {
    TextMessage::TextMessage(const QString& document, const QString& text) :
        Message(MessageType::text), document_(document), text_(text) {}

    QString TextMessage::document() const {
        return document_;
    }

    QString TextMessage::text() const {
        return text_;
    }

    QJsonObject TextMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["text"] = text_;
        return json_object;
    }
}
