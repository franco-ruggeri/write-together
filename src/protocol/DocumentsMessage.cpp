/*
 * Author: Franco Ruggeri
 */

#include "DocumentsMessage.h"

namespace collaborative_text_editor {
    DocumentsMessage::DocumentsMessage(std::vector<std::string> documents) :
        Message(MessageType::documents), documents_(documents) {}

    std::vector<std::string> DocumentsMessage::documents() const {
        return documents_;
    }
}
