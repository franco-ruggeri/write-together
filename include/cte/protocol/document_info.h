/*
 * Information about a document needed when opening.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class DocumentInfo {
        QList<Symbol> text_;
        int site_id_;                       // assigned to the client
        QHash<int,Symbol> cursors_;         // site_id -> symbol, for online users
        QHash<int,QString> usernames_;      // site_id -> username, for all users
        QHash<QString,Profile> profiles_;   // username -> profile, for all users
        QUrl sharing_link_;

    public:
        DocumentInfo();
        DocumentInfo(int site_id, const QUrl& sharing_link);
        DocumentInfo(const QList<Symbol>& text, int site_id, const QHash<int,Symbol>& cursors,
                     const QHash<int,QString>& usernames, const QHash<QString,Profile>& profiles,
                     const QUrl& sharing_link);
        explicit DocumentInfo(const QJsonObject& json_object);

        bool operator==(const DocumentInfo& other) const;

        QList<Symbol> text() const;
        int site_id() const;
        QHash<int,Symbol> cursors() const;
        QHash<int,QString> usernames() const;
        QHash<QString,Profile> profiles() const;
        QUrl sharing_link() const;

        QJsonObject json() const;
    };
}
