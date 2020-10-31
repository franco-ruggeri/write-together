/*
 * Root of the message hierarchy for the application protocol (see docs/protocol.md).
 * Some message types in the hierarchy are very similar or even identical, but they are kept separate for extensibility.
 *
 * Serialization is exposed by means of methods serialize() and deserialize(), hiding the internal choice of JSON.
 * Everything related to JSON is intentionally not declared public. In this way, server and client are forced to use
 * the methods serialize() and deserialize(), guaranteeing an easy future change (e.g. binary serialization).
 * Moreover, this class guarantees not to use new lines ('\n') in the serialization, so server and client can easily
 * exchange messages by lines.
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QJsonObject>
#include <QtCore/QByteArray>
#include <cte/protocol/message_type.h>
#include <optional>

namespace cte {
    class Message {
        MessageType type_;

    protected:
        explicit Message(MessageType type);
        virtual QJsonObject json() const;

    public:
        virtual bool operator==(const Message& other) const = 0;
        MessageType type() const;
        QByteArray serialize() const;
        static QSharedPointer<Message> deserialize(const QByteArray& serialized_message);
    };
}
