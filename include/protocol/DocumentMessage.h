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
        QHash<QString,int> site_ids_;       // site_id of all users with access to the document (even offline)
        QHash<QString,Profile> profiles_;   // profiles of all users with access to the document (even offline)
        QHash<QString,Symbol> cursors_;     // cursors of all users currently editing the document
        QString sharing_link_;

        DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document& document, const QVector<Symbol>& text, const QHash<QString,int>& site_ids,
                        const QHash<QString,Profile>& profiles, const QHash<QString,Symbol>& cursors,
                        const QString& sharing_link);
        bool operator==(const Message& other) const override;
        Document document() const;
        QVector<Symbol> text() const;
        QHash<QString,int> site_ids() const;
        QHash<QString,Profile> profiles() const;
        QHash<QString,Symbol> cursors() const;
        QString sharing_link() const;
    };
}
