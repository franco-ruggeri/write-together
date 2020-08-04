/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <editor/protocol/Message.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/DocumentData.h>
#include <editor/protocol/Profile.h>
#include <editor/crdt/Symbol.h>

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
