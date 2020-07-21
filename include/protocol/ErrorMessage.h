/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class ErrorMessage : public Message {
        std::string reason_;
    public:
        ErrorMessage(std::string reason);
        std::string reason() const;
    };
}
