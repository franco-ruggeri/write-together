/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/CreateMessage.h>

namespace cte {
    CreateMessage::CreateMessage(const QString& document_name) :
        Message(MessageType::create), document_name_(document_name) {}

    CreateMessage::CreateMessage(const QJsonObject& json_object) : Message(MessageType::create) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document_name");

        if (document_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        document_name_ = document_iterator->toString();

        if (document_name_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool CreateMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const CreateMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_name_ == o->document_name_;
    }

    QString CreateMessage::document_name() const {
        return document_name_;
    }

    QJsonObject CreateMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document_name"] = document_name_;
        return json_object;
    }
}
