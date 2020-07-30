/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QVector>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>
#include <protocol/Document.h>
#include <protocol/Profile.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class DocumentMessage : public Message {
        Document document_;
        QVector<Symbol> text_;
        QHash<int,Profile> users_;  // site_id -> Profile, for all users with access to the document
        QString sharing_link_;

        DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document &document, const QVector<Symbol> &text,
                        const QHash<int, Profile> &users, const QString &sharing_link);
        bool operator==(const Message& other) const override;
        Document document() const;
        QVector<Symbol> text() const;
        QHash<int,Profile> users() const;
        QString sharing_link() const;
    };
}
