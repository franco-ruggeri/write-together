/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/documents_message.h>
#include <QtCore/QJsonArray>

namespace cte {
    DocumentsMessage::DocumentsMessage() : Message(MessageType::documents) {}

    DocumentsMessage::DocumentsMessage(const QList<Document>& documents) :
        Message(MessageType::documents), documents_(documents) {}

    DocumentsMessage::DocumentsMessage(const QJsonObject &json_object) : Message(MessageType::documents) {
        auto end_iterator = json_object.end();
        auto documents_iterator = json_object.find("documents");

        if (documents_iterator != end_iterator) {
            if (!documents_iterator->isArray())
                throw std::logic_error("invalid message: invalid fields");

            documents_ = QList<Document>{};
            QJsonArray documents_json = documents_iterator->toArray();
            for (const auto& d_json : documents_json) {
                if (!d_json.isObject()) throw std::logic_error("invalid message: invalid fields");
                Document d(d_json.toObject());
                documents_->append(d);
            }
        }
    }

    bool DocumentsMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const DocumentsMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->documents_ == o->documents_;
    }
        
    std::optional<QList<Document>> DocumentsMessage::documents() const {
        return documents_;
    }

    QJsonObject DocumentsMessage::json() const {
        QJsonObject json_object = Message::json();
        if (documents_) {
            QJsonArray json_array;
            for (const auto &d : *documents_)
                json_array.push_back(d.json());
            json_object["documents"] = json_array;
        }
        return json_object;
    }
}
