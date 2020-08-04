/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>
#include <editor/protocol/Document.h>
#include <editor/crdt/Symbol.h>

namespace editor {
    class CursorMessage : public Message {
        Document document_;
        Symbol symbol_;
        std::optional<QString> username_;

        CursorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CursorMessage(const Document& document, const Symbol& symbol);
        CursorMessage(const Document& document, const Symbol& symbol, const QString& username);
        bool operator==(const Message& other) const override;
        Document document() const;
        Symbol symbol() const;
        std::optional<QString> username() const;
    };
}