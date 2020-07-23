/*
 * Author: Franco Ruggeri
 */

#include <protocol/CreateMessage.h>

namespace collaborative_text_editor {
    CreateMessage::CreateMessage(const QString& document) : Message(MessageType::create), document_(document) {}

    CreateMessage::CreateMessage(const QJsonObject &json_object) : Message(MessageType::create) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");

        if (document_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        document_ = document_iterator->toString();

        if (document_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool CreateMessage::operator==(const Message& other) const {
        const CreateMessage *o = dynamic_cast<const CreateMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_;
    }

    QString CreateMessage::document() const {
        return document_;
    }

    QJsonObject CreateMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        return json_object;
    }
}