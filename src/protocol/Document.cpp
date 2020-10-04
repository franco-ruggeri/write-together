/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/Document.h>
#include <random>

namespace cte {
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

    QUrl Document::generate_sharing_link(const Document& document) {
        static QString alphanum = "0123456789"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz";
        static int size = alphanum.size();

        QString random;
        for (int i=0; i<50; i++)
            random.push_back(alphanum[rand() % size]);

        return "cte:" + document.full_name() + "?" + random;
    }

    uint qHash(const Document& key, uint seed) {
        return qHash(key.owner(), seed) ^ qHash(key.name(), seed);
    }
}
