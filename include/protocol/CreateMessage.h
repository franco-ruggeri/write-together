/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class CreateMessage : public Message {
        std::string document_;
    public:
        CreateMessage(std::string document);
        std::string document() const;
    };
}
