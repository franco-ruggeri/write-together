/*
 * Identifier of a document. The document naming reflects a two-level directory structure, where each user has their
 * own directory (UFD).
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>

namespace cte {
    class Document {
        QString owner_;     // username of who creates the document
        QString name_;

        friend uint qHash(const Document &key, uint seed);

    public:
        Document();
        Document(const QString& owner, const QString& name);
        Document(const QString& full_name);
        explicit Document(const QJsonObject& json_object);

        bool operator==(const Document& other) const;

        QString owner() const;
        QString name() const;
        QString full_name() const;
        QJsonObject json() const;

        static QUrl generate_sharing_link(const Document& document);
    };

    uint qHash(const Document &key, uint seed);
}
