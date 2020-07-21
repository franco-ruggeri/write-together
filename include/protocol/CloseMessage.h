/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class CloseMessage : public Message {
        std::string document_, username_;
    public:
        CloseMessage(std::string document, std::string username);
        std::string document() const;
        std::string username() const;
    };
}
