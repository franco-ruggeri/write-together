/*
 * Message for the collaborative text editor application protocol.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include "MessageType.h"

namespace collaborative_text_editor {
    class Message {
        MessageType type_;

    protected:
        Message(MessageType type);

    public:
        MessageType type() const;
        virtual QJsonObject json() const;
    };
}
