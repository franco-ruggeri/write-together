/*
 * Format of a character in a document.
 */

#pragma once

#include <QtCore/QJsonObject>
#include <QtGui/QTextCharFormat>

namespace cte {
    class Format {
        bool bold_, italic_, underlined_;

    public:
        Format();
        Format(bool bold, bool italic, bool underlined);
        explicit Format(const QTextCharFormat& format);
        explicit Format(const QJsonObject& json_object);

        bool operator==(const Format& other) const;
        operator QTextCharFormat() const;

        QJsonObject json() const;

        bool bold() const;
        bool italic() const;
        bool underlined() const;
    };
}
