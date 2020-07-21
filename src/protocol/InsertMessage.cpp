/*
 * Author: Franco Ruggeri
 */

#include "InsertMessage.h"

namespace collaborative_text_editor {
    InsertMessage::InsertMessage(std::string document, Symbol symbol) :
        Message(MessageType::insert), document_(document), symbol_(symbol) {}

    std::string InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }
}
