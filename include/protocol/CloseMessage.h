/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include "Message.h"

namespace collaborative_text_editor {
    class CloseMessage : public Message {
        QString document_, username_;

    public:
        CloseMessage(QString document, QString username);
        QString document() const;
        QString username() const;
        QJsonObject json() const override;
    };
}
