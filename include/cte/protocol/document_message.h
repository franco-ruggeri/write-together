#pragma once

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class DocumentMessage : public Message {
        Document document_;
        DocumentInfo document_info_;

        explicit DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document& document, const DocumentInfo& document_info);
        bool operator==(const Message& other) const override;
        Document document() const;
        DocumentInfo document_info() const;
    };
}
