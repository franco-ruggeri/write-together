/*
 * Author: Franco Ruggeri
 */

#include "EraseMessage.h"

namespace collaborative_text_editor {
    EraseMessage::EraseMessage(std::string document, Symbol symbol) :
        Message(MessageType::erase), document_(document), symbol_(symbol) {}

    std::string EraseMessage::document() const {
        return document_;
    }

    Symbol EraseMessage::symbol() const {
        return symbol_;
    }
}
