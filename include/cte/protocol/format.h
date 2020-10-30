#pragma once

#include <QtCore/QJsonObject>

namespace cte {
    class Format {
        bool bold_, italic_, underlined_;

    public:
        Format();
        Format(bool bold, bool italic, bool underlined);
        explicit Format(const QJsonObject& json_object);

        bool operator==(const Format& other) const;

        QJsonObject json() const;

        bool bold() const;
        bool italic() const;
        bool underlined() const;
    };
}
