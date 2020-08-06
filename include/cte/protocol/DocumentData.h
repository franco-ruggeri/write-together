/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <QtCore/QJsonObject>
#include <cte/protocol/Profile.h>
#include <cte/crdt/Symbol.h>

namespace cte {
    class DocumentData {
        QList<Symbol> text_;
        int site_id_;                       // assigned to the client
        QHash<QString,Profile> profiles_;   // for all users with access to the document (including who is opening)
        QHash<QString,Symbol> cursors_;     // for other users currently editing the document
        QMultiHash<QString,int> site_ids_;  // for all users with access to the document (including who is opening)
        QString sharing_link_;

    public:
        DocumentData();
        DocumentData(int site_id, const QString& sharing_link);
        DocumentData(const QList<Symbol>& text, int site_id, const QHash<QString,Profile>& profiles,
                     const QHash<QString,Symbol>& cursors, const QMultiHash<QString,int>& site_ids,
                     const QString& sharing_link);
        explicit DocumentData(const QJsonObject& json_object);

        bool operator==(const DocumentData& other) const;

        QList<Symbol> text() const;
        int site_id() const;
        QHash<QString,Profile> profiles() const;
        QHash<QString,Symbol> cursors() const;
        QMultiHash<QString,int> site_ids() const;
        QString sharing_link() const;

        QJsonObject json() const;
    };
}
