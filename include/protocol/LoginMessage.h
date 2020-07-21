/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class LoginMessage : public Message {
        QString username_, password_;

        QJsonObject json() const override;
    public:
        LoginMessage(QString username, QString password);
        QString username() const;
        QString password() const;
    };
}
