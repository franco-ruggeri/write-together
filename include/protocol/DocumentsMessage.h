/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>
#include <QString>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        std::vector<QString> documents_;

        QJsonObject json() const override;
    public:
        DocumentsMessage(const std::vector<QString>& documents);
        std::vector<QString> documents() const;
    };
}
