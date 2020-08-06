/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QJsonObject>
#include <protocol/Profile.h>
#include <crdt/Symbol.h>


namespace editor {
    class DocumentData {
        QList<Symbol> text_;
        int site_id_user_, site_counter_user_;      // assigned to the client
        QHash<QString,int> site_id_others_;         // for all users with access to the document
        QHash<QString,Profile> profile_others_;     // for all users with access to the document
        QHash<QString,Symbol> cursors_;             // for other users currently editing the document
        QString sharing_link_;

    public:
        DocumentData();
        DocumentData(int site_id_user, int site_counter_user);
        DocumentData(const QList<Symbol>& text, int site_id_user, int site_counter_user,
                     const QHash<QString,int>& site_id_others, const QHash<QString,Profile>& profile_others,
                     const QHash<QString,Symbol>& cursors, const QString& sharing_link);
        DocumentData(const QJsonObject& json_object);

        bool operator==(const DocumentData& other) const;

        QList<Symbol> text() const;
        int site_id_user() const;
        int site_counter_user() const;
        QHash<QString,int> site_id_others() const;
        QHash<QString,Profile> profile_others() const;
        QHash<QString,Symbol> cursors() const;
        QString sharing_link() const;

        QJsonObject json() const;
        static QString generate_sharing_link();
    };
}