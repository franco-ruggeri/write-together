/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/EraseMessage.h>

namespace cte {
    EraseMessage::EraseMessage(const Document& document, const Symbol& symbol) :
        Message(MessageType::erase), document_(document), symbol_(symbol) {}

    EraseMessage::EraseMessage(const QJsonObject &json_object) : Message(MessageType::erase) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator  ||
            !document_iterator->isObject() || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        symbol_ = Symbol(symbol_iterator->toObject());
    }

    bool EraseMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const EraseMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->symbol_ == o->symbol_;
    }

    Document EraseMessage::document() const {
        return document_;
    }

    Symbol EraseMessage::symbol() const {
        return symbol_;
    }

    QJsonObject EraseMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
