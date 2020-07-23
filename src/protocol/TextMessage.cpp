/*
 * Author: Franco Ruggeri
 */

#include <protocol/TextMessage.h>

namespace collaborative_text_editor {
    TextMessage::TextMessage(const QString& document, const QString& text) :
        Message(MessageType::text), document_(document), text_(text) {}

    TextMessage::TextMessage(const QJsonObject &json_object) : Message(MessageType::text) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto text_iterator = json_object.find("text");

        if (document_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        document_ = document_iterator.value().toString();
        text_ = text_iterator->toString();

        if (document_.isNull() || text_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool TextMessage::operator==(const Message& other) const {
        const TextMessage *o = dynamic_cast<const TextMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_;
    }
        
    QString TextMessage::document() const {
        return document_;
    }

    QString TextMessage::text() const {
        return text_;
    }

    QJsonObject TextMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["text"] = text_;
        return json_object;
    }
}
