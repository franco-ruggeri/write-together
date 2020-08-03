/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>

namespace editor {
    class ErrorMessage : public Message {
        QString reason_;

        ErrorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        ErrorMessage(const QString& reason);
        bool operator==(const Message& other) const override;
        QString& reason();
    };
}
