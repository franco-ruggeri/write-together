/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class EraseMessage : public Message {
        std::string document_;
        Symbol symbol_;
    public:
        EraseMessage(std::string document, Symbol symbol);
        std::string document() const;
        Symbol symbol() const;
    };
}
