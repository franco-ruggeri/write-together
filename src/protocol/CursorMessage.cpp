/*
 * Author: Franco Ruggeri
 */

#include "CursorMessage.h"

namespace collaborative_text_editor {
    CursorMessage::CursorMessage(const QString& document, const QString& username, const Symbol& symbol) :
        Message(MessageType::cursor), document_(document), username_(username), symbol_(symbol) {}

    QString CursorMessage::document() const {
        return document_;
    }

    QString CursorMessage::username() const {
        return username_;
    }

    Symbol CursorMessage::symbol() const {
        return symbol_;
    }

    QJsonObject CursorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["username"] = username_;
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
