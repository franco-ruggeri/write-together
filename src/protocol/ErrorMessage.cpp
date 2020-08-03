/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/ErrorMessage.h>

namespace editor {
    ErrorMessage::ErrorMessage(const QString& reason) : Message(MessageType::error), reason_(reason) {}

    ErrorMessage::ErrorMessage(const QJsonObject &json_object) : Message(MessageType::error) {
        auto end_iterator = json_object.end();
        auto reason_iterator = json_object.find("reason");

        if (reason_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        reason_ = reason_iterator->toString();

        if (reason_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool ErrorMessage::operator==(const Message& other) const {
        const ErrorMessage *o = dynamic_cast<const ErrorMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
            this->reason_ == o->reason_;
    }

    QString& ErrorMessage::reason() {
        return reason_;
    }

    QJsonObject ErrorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["reason"] = reason_;
        return json_object;
    }
}
