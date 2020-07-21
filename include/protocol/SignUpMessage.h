/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class SignUpMessage : public Message {
        std::string username_, password_;
    public:
        SignUpMessage(std::string username, std::string password);
        std::string username() const;
        std::string password() const;
    };
}
