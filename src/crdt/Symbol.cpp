/*
 * Author: Franco Ruggeri
 */

#include <cte/crdt/Symbol.h>
#include <cte/crdt/SharedEditor.h>
#include <QtCore/QJsonArray>

namespace cte {
    Symbol::Symbol() :
        value_('\0'), site_id_(SharedEditor::invalid_site_id), site_counter_(SharedEditor::invalid_site_counter) {}

    Symbol::Symbol(QChar value, int site_id, int site_counter, const QVector<int>& position) :
        value_(value), site_id_(site_id), site_counter_(site_counter), position_(position) {}

    Symbol::Symbol(const QJsonObject &json_object) {
        auto end_iterator = json_object.end();
        auto value_iterator = json_object.find("value");
        auto site_id_iterator = json_object.find("site_id");
        auto site_counter_iterator = json_object.find("site_counter");
        auto position_iterator = json_object.find("position");

        if (value_iterator == end_iterator || site_id_iterator == end_iterator ||
            site_counter_iterator == end_iterator || position_iterator == end_iterator ||
            !position_iterator->isArray())
            throw std::logic_error("invalid message: invalid fields");

        QString v = value_iterator->toString();
        site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
        site_counter_ = site_counter_iterator->toInt(SharedEditor::invalid_site_counter);

        if (v.isNull() || v.size() != 1 || site_id_ == SharedEditor::invalid_site_id ||
            site_counter_ == SharedEditor::invalid_site_counter)
            throw std::logic_error("invalid message: invalid fields");

        value_ = v.at(0);

        QJsonArray position_json = position_iterator->toArray();
        for (const auto& p_json : position_json) {
            int p = p_json.toInt(-1);
            if (p == -1) throw std::logic_error("invalid message: invalid fields");
            position_.push_back(p);
        }
    }

    bool Symbol::operator<(const Symbol& other) const {
        return this->position_ < other.position_ ||
            (this->position_ == other.position_ && this->site_id_ < other.site_id_);
    }

    bool Symbol::operator==(const Symbol &other) const {
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

    QVector<int> Symbol::position() const {
        return position_;
    }

    QJsonObject Symbol::json() const {
        QJsonObject json_object;

        // basic types
        json_object["value"] = QString(value_);
        json_object["site_id"] = site_id_;
        json_object["site_counter"] = site_counter_;

        // position array
        QJsonArray json_position;
        for (const auto& p : position_)
            json_position.push_back(p);
        json_object["position"] = json_position;

        return json_object;
    }
}
