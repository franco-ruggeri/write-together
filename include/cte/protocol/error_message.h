/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>

namespace cte {
    class ErrorMessage : public Message {
        QString reason_;

        explicit ErrorMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        explicit ErrorMessage(const QString& reason);
        bool operator==(const Message& other) const override;
        QString reason() const;
    };
}
