/*
 * Identifier of a document.
 *
 * Author: Franco Ruggeri
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
        explicit Document(const QJsonObject& json_object);

        bool operator==(const Document& other) const;

        QString owner() const;
        QString name() const;
        QString full_name() const;
        QJsonObject json() const;

        static QUrl generate_sharing_link(const Document& document);
    };
    inline uint qHash(const Document &key, uint seed){
        return qHash(key.owner(), seed) ^ qHash(key.name(), seed);
    }
}
