/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>
#include <editor/protocol/Document.h>
#include <editor/crdt/Symbol.h>

namespace editor {
    class InsertMessage : public Message {
        Document document_;
        Symbol symbol_;

        InsertMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        InsertMessage(const Document& document, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        Document& document();
        Symbol& symbol();
    };
}