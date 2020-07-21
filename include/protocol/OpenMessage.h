/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class OpenMessage : public Message {
        std::string document_;
    public:
        OpenMessage(std::string document);
        std::string document() const;
    };
}
