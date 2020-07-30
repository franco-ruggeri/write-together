/*
 * Author: Franco Ruggeri
 */

#include <protocol/CursorMessage.h>

namespace collaborative_text_editor {
    CursorMessage::CursorMessage(const Document& document, const QString& username, const Symbol& symbol) :
        Message(MessageType::cursor), document_(document), username_(username), symbol_(symbol) {}

    CursorMessage::CursorMessage(const QJsonObject &json_object) : Message(MessageType::cursor) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto username_iterator = json_object.find("username");
        auto symbol_iterator = json_object.find("symbol");

        if (document_iterator == end_iterator || username_iterator == end_iterator || symbol_iterator == end_iterator ||
            !document_iterator->isObject() || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        username_ = username_iterator->toString();
        symbol_ = Symbol(symbol_iterator->toObject());

        if (username_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool CursorMessage::operator==(const Message& other) const {
        const CursorMessage *o = dynamic_cast<const CursorMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->username_ == o->username_ && this->symbol_ == o->symbol_;
    }

    Document CursorMessage::document() const {
        return document_;
    }

    QString CursorMessage::username() const {
        return username_;
    }

    Symbol CursorMessage::symbol() const {
        return symbol_;
    }

    QJsonObject CursorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["username"] = username_;
        json_object["symbol"] = symbol_.json();
        return json_object;
    }
}
