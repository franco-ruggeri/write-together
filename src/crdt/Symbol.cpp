/*
 * Author: Franco Ruggeri
 */

#include "Symbol.h"
#include <QJsonArray>

namespace collaborative_text_editor {
    Symbol::Symbol(QChar value, int site_id, int site_counter, const std::vector<int>& position) :
        value_(value), site_id_(site_id), site_counter_(site_counter), position_(position) {}

    bool Symbol::operator<(const Symbol& other) {
        return this->position_ < other.position_ ||
            (this->position_ == other.position_ && this->site_id_ < other.site_id_);
    }

    bool Symbol::operator==(const Symbol &other) {
        return this->site_id_ == other.site_id_ && this->site_counter_ == other.site_counter_ &&
               this->position_ == other.position_;
    }

    QChar Symbol::value() const {
        return value_;
    }

    int Symbol::site_id() const {
        return site_id_;
    }

    int Symbol::site_counter() const {
        return site_counter_;
    }

    std::vector<int> Symbol::position() const {
        return position_;
    }

    QJsonObject Symbol::json() const {
        QJsonObject json_;

        // basic types
        json_.insert("value", QString(value_));
        json_.insert("site_id", site_id_);
        json_.insert("site_counter", site_counter_);

        // position array
        QJsonArray json_position;
        for (const auto& p : position_)
            json_position.push_back(p);
        json_.insert("position", json_position);

        return json_;
    }
}
