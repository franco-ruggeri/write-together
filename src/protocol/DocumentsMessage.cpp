/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentsMessage.h>
#include <QJsonArray>

namespace collaborative_text_editor {
    DocumentsMessage::DocumentsMessage(const std::vector<QString>& documents) :
        Message(MessageType::documents), documents_(documents) {}

    DocumentsMessage::DocumentsMessage(const QJsonObject &json_object) : Message(MessageType::documents) {
        auto end_iterator = json_object.end();
        auto documents_iterator = json_object.find("documents");

        if (documents_iterator == end_iterator || !documents_iterator->isArray())
            throw std::logic_error("invalid message: invalid fields");

        QJsonArray documents_json = documents_iterator->toArray();
        for (const auto& d_json : documents_json) {
            QString d = d_json.toString();
            if (d.isNull()) throw std::logic_error("invalid message: invalid fields");
            documents_.push_back(d);
        }
    }

    bool DocumentsMessage::operator==(const Message& other) const {
        const DocumentsMessage *o = dynamic_cast<const DocumentsMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->documents_ == o->documents_;
    }
        
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
