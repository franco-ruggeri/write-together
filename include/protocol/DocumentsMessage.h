/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>
#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        std::vector<std::string> documents_;
    public:
        DocumentsMessage(std::vector<std::string> documents);
        std::vector<std::string> documents() const;
    };
}
