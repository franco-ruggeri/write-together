/*
 * Author: Franco Ruggeri
 */

#include "InsertMessage.h"

namespace collaborative_text_editor {
    InsertMessage::InsertMessage(QString document, Symbol symbol) :
        Message(MessageType::insert), document_(document), symbol_(symbol) {}

    QString InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }

    QJsonObject InsertMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        json_.insert("symbol", symbol_.json());
        return json_;
    }
}
