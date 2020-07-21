/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"
#include "Symbol.h"

namespace collaborative_text_editor {
    class CursorMessage : public Message {
        std::string document_, username_;
        Symbol symbol_;
    public:
        CursorMessage(std::string document, std::string username, Symbol symbol);
        std::string document() const;
        std::string username() const;
        Symbol symbol() const;
    };
}
