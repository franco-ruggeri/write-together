/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/DocumentData.h>
#include <cte/protocol/Profile.h>
#include <cte/crdt/Symbol.h>

namespace cte {
    class DocumentMessage : public Message {
        Document document_;
        DocumentData document_data_;

        explicit DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document& document, const DocumentData& document_data);
        bool operator==(const Message& other) const override;
        Document document() const;
        DocumentData document_data() const;
    };
}
