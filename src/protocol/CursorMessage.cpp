/*
 * Author: Franco Ruggeri
 */

#include "CursorMessage.h"

namespace collaborative_text_editor {
    CursorMessage::CursorMessage(QString document, QString username, Symbol symbol) :
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
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        json_.insert("username", username_);
        json_.insert("symbol", symbol_.json());
        return json_;
    }
}
