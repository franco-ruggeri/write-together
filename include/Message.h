/*
 * Message for the collaborative text editor application protocol.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "MessageType.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class Message {
        int source_site_id_;
        MessageType type_;
        Symbol symbol_;

    public:
        Message(int source_site_id, MessageType type, const Symbol& symbol);

        int source() const;
        MessageType type() const;
        Symbol symbol() const;
    };
}
