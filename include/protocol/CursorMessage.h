/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class CursorMessage : public Message {
        Document document_;
        QString username_;
        Symbol symbol_;

        CursorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CursorMessage(const Document& document, const QString& username, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        Document document() const;
        QString username() const;
        Symbol symbol() const;
    };
}
