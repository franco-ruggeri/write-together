/*
 * Author: Franco Ruggeri
 */

#include <protocol/Document.h>
#include <boost/functional/hash.hpp>

namespace collaborative_text_editor {
    Document::Document() {}

    Document::Document(const QString& owner, const QString& name) :
            owner_(owner), name_(name) {}

    Document::Document(const QJsonObject& json_object) {
        auto end_iterator = json_object.end();
        auto owner_iterator = json_object.find("owner");
        auto document_name_iterator = json_object.find("name");

        if (owner_iterator == end_iterator || document_name_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        owner_ = owner_iterator->toString();
        name_ = document_name_iterator->toString();

        if (owner_.isNull() || name_.isNull())
            throw std::logic_error("invalid message: invalid fields");
    }

    bool Document::operator==(const Document& other) const {
        return this->owner_ == other.owner_ && this->name_ == other.name_;
    }

    QString Document::owner() const {
        return owner_;
    }

    QString Document::name() const {
        return name_;
    }

    QString Document::full_name() const {
        return owner_ + "/" + name_;
    }

    QJsonObject Document::json() const {
        QJsonObject json_object;
        json_object["owner"] = owner_;
        json_object["name"] = name_;
        return json_object;
    }

    uint qHash(const Document &key, uint seed) {
        size_t s = static_cast<size_t>(seed);
        boost::hash_combine(s, key.owner_.toStdString());
        boost::hash_combine(s, key.name_.toStdString());
        return s;
    }
}
