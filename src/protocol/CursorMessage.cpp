/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/CursorMessage.h>

namespace editor {
    CursorMessage::CursorMessage(const Document &document, const Symbol &symbol) :
        Message(MessageType::cursor), document_(document), symbol_(symbol) {}

    CursorMessage::CursorMessage(const Document &document, const Symbol &symbol, const QString& username) :
        Message(MessageType::cursor), document_(document), symbol_(symbol), username_(username) {}

    CursorMessage::CursorMessage(const QJsonObject &json_object) : Message(MessageType::cursor) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto symbol_iterator = json_object.find("symbol");
        auto username_iterator = json_object.find("username");

        if (document_iterator == end_iterator || symbol_iterator == end_iterator ||
            !document_iterator->isObject() || !symbol_iterator->isObject())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        symbol_ = Symbol(symbol_iterator->toObject());

        if (username_iterator != end_iterator) {
            username_ = username_iterator->toString();
            if (username_->isNull())
                throw std::logic_error("invalid message: invalid fields");
        }
    }

    bool CursorMessage::operator==(const Message& other) const {
        const CursorMessage *o = dynamic_cast<const CursorMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->username_ == o->username_ && this->symbol_ == o->symbol_;
    }

    Document& CursorMessage::document() {
        return document_;
    }

    Symbol& CursorMessage::symbol() {
        return symbol_;
    }

    std::optional<QString>& CursorMessage::username() {
        return username_;
    }

    QJsonObject CursorMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["symbol"] = symbol_.json();
        if (username_) json_object["username"] = *username_;
        return json_object;
    }
}
