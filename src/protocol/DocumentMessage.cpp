/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>

namespace editor {
    DocumentMessage::DocumentMessage(const Document& document, const DocumentData& document_data) :
        Message(MessageType::document), document_(document), document_data_(document_data) {}

    DocumentMessage::DocumentMessage(const QJsonObject &json_object) : Message(MessageType::document) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto document_data_iterator = json_object.find("document_data");

        if (document_iterator == end_iterator || document_data_iterator == end_iterator ||
            !document_iterator->isObject() || !document_data_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        document_data_ = DocumentData(document_data_iterator->toObject());
    }

    bool DocumentMessage::operator==(const Message& other) const {
        const DocumentMessage *o = dynamic_cast<const DocumentMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->document_data_ == o->document_data_;
    }

    Document DocumentMessage::document() const {
        return document_;
    }

    DocumentData DocumentMessage::document_data() const {
        return document_data_;
    }

    QJsonObject DocumentMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["document_data"] = document_data_.json();
        return json_object;
    }
}
