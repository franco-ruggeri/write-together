/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class CloseMessage : public Message {
        QString document_, username_;

        QJsonObject json() const override;
    public:
        CloseMessage(const QString& document, const QString& username);
        QString document() const;
        QString username() const;
    };
}
