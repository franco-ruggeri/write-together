/*
 * Author: Franco Ruggeri
 */

#include "InsertMessage.h"

namespace collaborative_text_editor {
    InsertMessage::InsertMessage(const QString& document, const Symbol& symbol) :
        Message(MessageType::insert), document_(document), symbol_(symbol) {}

    QString InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }

    QJsonObject InsertMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
