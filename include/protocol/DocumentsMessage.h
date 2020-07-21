/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>
#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        std::vector<QString> documents_;

    public:
        DocumentsMessage(std::vector<QString> documents);
        std::vector<QString> documents() const;
        QJsonObject json() const override;
    };
}
