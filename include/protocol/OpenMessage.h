/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <string>
#include "Message.h"

namespace collaborative_text_editor {
    class OpenMessage : public Message {
        QString document_;

    public:
        OpenMessage(QString document);
        QString document() const;
        QJsonObject json() const override;
    };
}
