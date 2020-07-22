/*
 * Author: Franco Ruggeri
 */

#include "DocumentsMessage.h"
#include <QJsonArray>

namespace collaborative_text_editor {
    DocumentsMessage::DocumentsMessage(const std::vector<QString>& documents) :
        Message(MessageType::documents), documents_(documents) {}

    std::vector<QString> DocumentsMessage::documents() const {
        return documents_;
    }

    QJsonObject DocumentsMessage::json() const {
        QJsonObject json_object = Message::json();
        QJsonArray json_array;
        for (const auto& d : documents_)
            json_array.push_back(d);
        json_object["documents"] = json_array;
        return json_object;
    }
}
