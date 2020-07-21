/*
 * Author: Franco Ruggeri
 */

#include "DocumentsMessage.h"
#include <QJsonArray>

namespace collaborative_text_editor {
    DocumentsMessage::DocumentsMessage(std::vector<QString> documents) :
        Message(MessageType::documents), documents_(documents) {}

    std::vector<QString> DocumentsMessage::documents() const {
        return documents_;
    }

    QJsonObject DocumentsMessage::json() const {
        QJsonObject json_ = Message::json();

        // documents array
        QJsonArray json_documents;
        for (const auto& d : documents_)
            json_documents.push_back(d);
        json_.insert("position", json_documents);

        return json_;
    }
}
