/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class OpenMessage : public Message {
        QString document_;

        OpenMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        OpenMessage(const QString& document);
        bool operator==(const Message& other) const override;
        QString document() const;
    };
}
