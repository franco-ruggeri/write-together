/*
 * Author: Franco Ruggeri
 */

#include <protocol/InsertMessage.h>

namespace collaborative_text_editor {
    InsertMessage::InsertMessage(const QString& document, const Symbol& symbol) :
        Message(MessageType::insert), document_(document), symbol_(symbol) {}

    InsertMessage::InsertMessage(const QJsonObject &json_object) : Message(MessageType::insert) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator  || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = document_iterator->toString();
        symbol_.from_json(symbol_iterator->toObject());

        if (document_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool InsertMessage::operator==(const Message& other) const {
        const InsertMessage *o = dynamic_cast<const InsertMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->symbol_ == o->symbol_;
    }
        
    QString InsertMessage::document() const {
        return document_;
    }

    Symbol InsertMessage::symbol() const {
        return symbol_;
    }

    QJsonObject InsertMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_;
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
