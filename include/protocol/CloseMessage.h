/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class CloseMessage : public Message {
        QString document_, username_;

        CloseMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        CloseMessage(const QString& document, const QString& username);
        bool operator==(const Message& other) const override;
        QString document() const;
        QString username() const;
    };
}
