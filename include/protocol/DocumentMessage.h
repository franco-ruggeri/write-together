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
#include <protocol/Profile.h>
#include <crdt/Symbol.h>

namespace collaborative_text_editor {
    class DocumentMessage : public Message {
        Document document_;
        QList<Symbol> text_;
        QHash<QString,int> site_ids_;       // site_id of all users with access to the document (even offline)
        QHash<QString,Profile> profiles_;   // profiles of all users with access to the document (even offline)
        QHash<QString,Symbol> cursors_;     // cursors of all users currently editing the document
        QString sharing_link_;

        DocumentMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentMessage(const Document& document, const QList<Symbol>& text, const QHash<QString,int>& site_ids,
                        const QHash<QString,Profile>& profiles, const QHash<QString,Symbol>& cursors,
                        const QString& sharing_link);
        bool operator==(const Message& other) const override;
        Document& document();
        QList<Symbol>& text();
        QHash<QString,int>& site_ids();
        QHash<QString,Profile>& profiles();
        QHash<QString,Symbol>& cursors();
        QString& sharing_link();
    };
}
