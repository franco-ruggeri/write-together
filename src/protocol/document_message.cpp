/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/document_message.h>

namespace cte {
    DocumentMessage::DocumentMessage(const Document& document, const DocumentInfo& document_info) :
            Message(MessageType::document), document_(document), document_info_(document_info) {}

    DocumentMessage::DocumentMessage(const QJsonObject &json_object) : Message(MessageType::document) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto document_data_iterator = json_object.find("document_info");

        if (document_iterator == end_iterator || document_data_iterator == end_iterator ||
            !document_iterator->isObject() || !document_data_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        document_info_ = DocumentInfo(document_data_iterator->toObject());
    }

    bool DocumentMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const DocumentMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->document_info_ == o->document_info_;
    }

    Document DocumentMessage::document() const {
        return document_;
    }

    DocumentInfo DocumentMessage::document_info() const {
        return document_info_;
    }

    QJsonObject DocumentMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["document_info"] = document_info_.json();
        return json_object;
    }
}
