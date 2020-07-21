/*
 * Root of the message hierarchy for the collaborative text editor application protocol.
 * Some message types in the hierarchy are very similar or even identical, but they are kept separate for extensibility.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QJsonObject>
#include <QByteArray>
#include "MessageType.h"

namespace collaborative_text_editor {
    class Message {
        MessageType type_;

    protected:
        Message(MessageType type);
        virtual QJsonObject json() const;

    public:
        MessageType type() const;
        QByteArray serialize() const;
    };
}
