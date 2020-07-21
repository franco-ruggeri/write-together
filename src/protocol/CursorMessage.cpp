/*
 * Author: Franco Ruggeri
 */

#include "CursorMessage.h"

namespace collaborative_text_editor {
    CursorMessage::CursorMessage(std::string document, std::string username, Symbol symbol) :
        Message(MessageType::cursor), document_(document), username_(username), symbol_(symbol) {}

    std::string CursorMessage::document() const {
        return document_;
    }

    std::string CursorMessage::username() const {
        return username_;
    }

    Symbol CursorMessage::symbol() const {
        return symbol_;
    }
}
