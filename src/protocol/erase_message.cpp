#include <cte/protocol/erase_message.h>
#include <cte/crdt/shared_editor.h>

namespace cte {
    EraseMessage::EraseMessage(const Document& document, const Symbol& symbol) :
        Message(MessageType::erase), document_(document), symbol_(symbol) {}

    EraseMessage::EraseMessage(const Document& document, const Symbol& symbol, int site_id) :
        Message(MessageType::erase), document_(document), symbol_(symbol), site_id_(site_id) {}

    EraseMessage::EraseMessage(const QJsonObject &json_object) : Message(MessageType::erase) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");
        auto site_id_iterator = json_object.find("site_id");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator  ||
            !document_iterator->isObject() || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        symbol_ = Symbol(symbol_iterator->toObject());

        if (site_id_iterator != end_iterator) {
            site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
            if (*site_id_ == SharedEditor::invalid_site_id)
                throw std::logic_error("invalid message: invalid fields");
        }
    }

    bool EraseMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const EraseMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->site_id_ == o->site_id_ && this->symbol_ == o->symbol_;
    }

    Document EraseMessage::document() const {
        return document_;
    }

    Symbol EraseMessage::symbol() const {
        return symbol_;
    }

    std::optional<int> EraseMessage::site_id() const {
        return site_id_;
    }

    QJsonObject EraseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        if (site_id_) json_object["site_id"] = *site_id_;
        return json_object;
    }
}
