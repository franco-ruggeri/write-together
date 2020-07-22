/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class SignUpMessage : public Message {
        QString username_, password_;

        QJsonObject json() const override;
    public:
        SignUpMessage(const QString& username, const QString& password);
        QString username() const;
        QString password() const;
    };
}
