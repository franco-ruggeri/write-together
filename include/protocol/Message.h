/*
 * Root of the message hierarchy for the collaborative text editor application protocol. Some message types in the
 * hierarchy are very similar or even identical, but they are kept separate for extensibility.
 *
 * Serialization is exposed by means of methods serialize() and deserialize(), hiding the internal choice of JSON.
 * Everything related to JSON is intentionally not declared public. In this way, server and client are forced to use
 * the methods serialize() and deserialize(), guaranteeing an easy future change (e.g. binary serialization).
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <memory>
#include <QJsonObject>
#include <QByteArray>
#include <protocol/MessageType.h>

namespace collaborative_text_editor {
    class Message {
        MessageType type_;

    protected:
        Message(MessageType type);
        virtual QJsonObject json() const;

    public:
        virtual bool operator==(const Message& other) const = 0;
        MessageType type() const;
        QByteArray serialize() const;
        static std::shared_ptr<Message> deserialize(const QByteArray& serialized_message);
    };
}
