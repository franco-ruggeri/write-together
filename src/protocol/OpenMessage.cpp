/*
 * Author: Franco Ruggeri
 */

#include <protocol/OpenMessage.h>

namespace collaborative_text_editor {
    OpenMessage::OpenMessage(const QString& document) : Message(MessageType::open), document_(document) {}

    OpenMessage::OpenMessage(const QJsonObject &json_object) : Message(MessageType::open) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");

        if (document_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        document_ = document_iterator->toString();

        if (document_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool OpenMessage::operator==(const Message& other) const {
        const OpenMessage *o = dynamic_cast<const OpenMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_;
    }
    
    QString OpenMessage::document() const {
        return document_;
    }

    QJsonObject OpenMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        return json_object;
    }
}
