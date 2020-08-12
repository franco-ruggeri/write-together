/*
 * Author: Franco Ruggeri
 */
#include <cte/protocol/CloseMessage.h>

namespace cte {
    CloseMessage::CloseMessage(const Document& document) : Message(MessageType::close), document_(document) {}

    CloseMessage::CloseMessage(const Document& document, const QString& username) :
            Message(MessageType::close), document_(document), username_(username) {}

    CloseMessage::CloseMessage(const QJsonObject &json_object) : Message(MessageType::close) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto username_iterator = json_object.find("username");

        if (document_iterator == end_iterator || !document_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        if (username_iterator != end_iterator) {
            username_ = username_iterator->toString();
            if (username_->isNull())
                throw std::logic_error("invalid message: invalid fields");
        }
    }

    bool CloseMessage::operator==(const Message& other) const {
        const CloseMessage *o = dynamic_cast<const CloseMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->username_ == o->username_;
    }

    Document CloseMessage::document() const {
        return document_;
    }

    std::optional<QString> CloseMessage::username() const {
        return username_;
    }

    QJsonObject CloseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        if (username_) json_object["username"] = *username_;
        return json_object;
    }
}
