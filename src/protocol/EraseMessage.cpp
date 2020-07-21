/*
 * Author: Franco Ruggeri
 */

#include "EraseMessage.h"

namespace collaborative_text_editor {
    EraseMessage::EraseMessage(QString document, Symbol symbol) :
        Message(MessageType::erase), document_(document), symbol_(symbol) {}

    QString EraseMessage::document() const {
        return document_;
    }

    Symbol EraseMessage::symbol() const {
        return symbol_;
    }
    
    QJsonObject EraseMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        json_.insert("symbol", symbol_.json());
        return json_;
    }
}
