#include <cte/protocol/insert_message.h>

namespace cte {
    InsertMessage::InsertMessage(const Document& document, const Symbol& symbol, const Format& format) :
        Message(MessageType::insert), document_(document), symbol_(symbol), format_(format) {}

    InsertMessage::InsertMessage(const QJsonObject &json_object) : Message(MessageType::insert) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");
        auto format_iterator = json_object.find("format");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator || format_iterator == end_iterator ||
            !document_iterator->isObject() || !symbol_iterator->isObject() || !format_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        symbol_ = Symbol(symbol_iterator->toObject());
        format_ = Format(format_iterator->toObject());
    }

    bool InsertMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const InsertMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->symbol_ == o->symbol_ && this->format_ == o->format_;
    }

    Document InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }

    Format InsertMessage::format() const {
        return format_;
    }

    QJsonObject InsertMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        json_object["format"] = format_.json();
        return json_object;
    }
}
