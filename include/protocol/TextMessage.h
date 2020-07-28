/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class TextMessage : public Message {
        QString document_, text_;

        TextMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        TextMessage(const QString& document, const QString& text);
        bool operator==(const Message& other) const override;
        QString document() const;
        QString text() const;
    };
}
