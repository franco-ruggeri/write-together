/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <protocol/DocumentData.h>
#include <protocol/Profile.h>
#include <crdt/Symbol.h>

namespace editor {
    class DocumentMessage : public Message {
        Document document_;
        DocumentData document_data_;

        DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document& document, const DocumentData& document_data);
        bool operator==(const Message& other) const override;
        Document document() const;
        DocumentData document_data() const;
    };
}
