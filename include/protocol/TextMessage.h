/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class TextMessage : public Message {
        std::string document_, text_;
    public:
        TextMessage(std::string document, std::string text);
        std::string document() const;
        std::string text() const;
    };
}
