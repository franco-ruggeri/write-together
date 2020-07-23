/*
 * Author: Franco Ruggeri
 */

#include <protocol/CloseMessage.h>

namespace collaborative_text_editor {
    CloseMessage::CloseMessage(const QString& document, const QString& username) :
        Message(MessageType::close), document_(document), username_(username) {}

    CloseMessage::CloseMessage(const QJsonObject &json_object) : Message(MessageType::close) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto username_iterator = json_object.find("username");

        if (document_iterator == end_iterator || username_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        document_ = document_iterator->toString();
        username_ = username_iterator->toString();

        if (document_.isNull() || username_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool CloseMessage::operator==(const Message& other) const {
        const CloseMessage *o = dynamic_cast<const CloseMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
            this->document_ == o->document_ && this->username_ == o->username_;
    }

    QString CloseMessage::document() const {
        return document_;
    }

    QString CloseMessage::username() const {
        return username_;
    }

    QJsonObject CloseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["username"] = username_;
        return json_object;
    }
}
