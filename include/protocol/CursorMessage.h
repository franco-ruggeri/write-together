/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class CursorMessage : public Message {
        QString document_, username_;
        Symbol symbol_;

        CursorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CursorMessage(const QString& document, const QString& username, const Symbol& symbol);
        bool operator==(const Message& other) const override;
        QString document() const;
        QString username() const;
        Symbol symbol() const;
    };
}
