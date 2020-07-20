/*
 * Author: Franco Ruggeri
 */

#include "Message.h"

namespace collaborative_text_editor {
    Message::Message(int source_site_id, MessageType type, const Symbol& symbol) :
        source_site_id_(source_site_id), type_(type), symbol(symbol) {}

    int Message::source() const {
        return source_site_id_;
    }

    MessageType Message::type() const {
        return type_;
    }

    Symbol Message::symbol() const {
        return symbol_;
    }
}
