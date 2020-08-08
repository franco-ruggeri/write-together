/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/InsertMessage.h>

namespace cte {
    InsertMessage::InsertMessage(const Document& document, const Symbol& symbol) :
        Message(MessageType::insert), document_(document), symbol_(symbol) {}

    InsertMessage::InsertMessage(const QJsonObject &json_object) : Message(MessageType::insert) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator ||
            !document_iterator->isObject() || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        symbol_ = Symbol(symbol_iterator->toObject());
    }

    bool InsertMessage::operator==(const Message& other) const {
        const InsertMessage *o = dynamic_cast<const InsertMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->symbol_ == o->symbol_;
    }

    Document InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }

    QJsonObject InsertMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
