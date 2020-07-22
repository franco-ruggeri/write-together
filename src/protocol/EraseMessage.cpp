/*
 * Author: Franco Ruggeri
 */

#include "EraseMessage.h"

namespace collaborative_text_editor {
    EraseMessage::EraseMessage(const QString& document, const Symbol& symbol) :
        Message(MessageType::erase), document_(document), symbol_(symbol) {}

    QString EraseMessage::document() const {
        return document_;
    }

    Symbol EraseMessage::symbol() const {
        return symbol_;
    }

    QJsonObject EraseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
