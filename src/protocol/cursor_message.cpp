#include <cte/protocol/cursor_message.h>
#include <cte/crdt/shared_editor.h>

namespace cte {
    CursorMessage::CursorMessage(const Document &document, const Symbol &symbol) :
            Message(MessageType::cursor), document_(document), symbol_(symbol) {}

    CursorMessage::CursorMessage(const Document &document, const Symbol &symbol, int site_id) :
            Message(MessageType::cursor), document_(document), symbol_(symbol), site_id_(site_id) {}

    CursorMessage::CursorMessage(const QJsonObject &json_object) : Message(MessageType::cursor) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");
        auto site_id_iterator = json_object.find("site_id");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator ||
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

    bool CursorMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const CursorMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->site_id_ == o->site_id_ && this->symbol_ == o->symbol_;
    }

    Document CursorMessage::document() const {
        return document_;
    }

    Symbol CursorMessage::symbol() const {
        return symbol_;
    }

    std::optional<int> CursorMessage::site_id() const {
        return site_id_;
    }

    QJsonObject CursorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        if (site_id_) json_object["site_id"] = *site_id_;
        return json_object;
    }
}