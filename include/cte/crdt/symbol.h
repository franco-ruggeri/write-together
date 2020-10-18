/*
 * Encapsulation of a single character in the text cte with all the information needed for CRDT.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QVector>
#include <QtCore/QChar>
#include <QtCore/QJsonObject>

namespace cte {
    class Symbol {
        QChar value_;
        int site_id_;
        int site_counter_;
        QVector<int> position_;

    public:
        Symbol();
        Symbol(QChar value, int site_id, int site_counter, const QVector<int>& position);
        explicit Symbol(const QJsonObject& json_object);

        bool operator<(const Symbol& other) const;
        bool operator==(const Symbol& other) const;

        QChar value() const;
        int site_id() const;
        int site_counter() const;
        QVector<int> position() const;

        QJsonObject json() const;
    };
}
