/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>

namespace collaborative_text_editor {
    class Document {
        QString owner_;     // username of who creates the document
        QString name_;

        friend uint qHash(const Document &key, uint seed);

    public:
        Document();
        Document(const QString& owner, const QString& name);
        Document(const QJsonObject& json_object);

        bool operator==(const Document& other) const;

        QString owner() const;
        QString name() const;

        QJsonObject json() const;
    };
}
