#include <cte/protocol/format_message.h>

namespace cte {
    FormatMessage::FormatMessage(const Document& document, const Symbol& symbol, const Format& format) :
        Message(MessageType::format), document_(document), symbol_(symbol), format_(format) {}

    FormatMessage::FormatMessage(const QJsonObject &json_object) : Message(MessageType::format) {
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

    bool FormatMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const FormatMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->symbol_ == o->symbol_ && this->format_ == o->format_;
    }

    Document FormatMessage::document() const {
        return document_;
    }

    Symbol FormatMessage::symbol() const {
        return symbol_;
    }

    Format FormatMessage::format() const {
        return format_;
    }

    QJsonObject FormatMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        json_object["format"] = format_.json();
        return json_object;
    }
}
