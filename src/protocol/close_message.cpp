#include <cte/protocol/close_message.h>
#include <cte/crdt/shared_editor.h>

namespace cte {
    CloseMessage::CloseMessage(const Document& document) : Message(MessageType::close), document_(document) {}

    CloseMessage::CloseMessage(const Document& document, int site_id) :
            Message(MessageType::close), document_(document), site_id_(site_id) {}

    CloseMessage::CloseMessage(const QJsonObject &json_object) : Message(MessageType::close) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto site_id_iterator = json_object.find("site_id");

        if (document_iterator == end_iterator || !document_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());

        if (site_id_iterator != end_iterator) {
            site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
            if (*site_id_ == SharedEditor::invalid_site_id)
                throw std::logic_error("invalid message: invalid fields");
        }
    }

    bool CloseMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const CloseMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->site_id_ == o->site_id_;
    }

    Document CloseMessage::document() const {
        return document_;
    }

    std::optional<int> CloseMessage::site_id() const {
        return site_id_;
    }

    QJsonObject CloseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        if (site_id_) json_object["site_id"] = *site_id_;
        return json_object;
    }
}
