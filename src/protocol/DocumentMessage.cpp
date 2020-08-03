/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/DocumentMessage.h>
#include <editor/crdt/SharedEditor.h>
#include <QtCore/QJsonArray>

namespace editor {
    DocumentMessage::DocumentMessage(const Document& document, const QList<Symbol>& text, int site_id, int site_counter,
                                     const QHash<QString,int>& site_ids, const QHash<QString,Profile>& profiles,
                                     const QHash<QString,Symbol>& cursors, const QString& sharing_link) :
        Message(MessageType::document), document_(document), text_(text), site_id_(site_id),
        site_counter_(site_counter), site_ids_(site_ids), profiles_(profiles), cursors_(cursors),
        sharing_link_(sharing_link) {}

    DocumentMessage::DocumentMessage(const QJsonObject &json_object) : Message(MessageType::document) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto text_iterator = json_object.find("text");
        auto site_id_iterator = json_object.find("site_id");
        auto site_counter_iterator = json_object.find("site_counter");
        auto site_ids_iterator = json_object.find("site_ids");
        auto profiles_iterator = json_object.find("profiles");
        auto cursors_iterator = json_object.find("cursors");
        auto sharing_link_iterator = json_object.find("sharing_link");

        if (document_iterator == end_iterator || text_iterator == end_iterator ||
            site_id_iterator == end_iterator || site_counter_iterator == end_iterator ||
            site_ids_iterator == end_iterator || profiles_iterator == end_iterator ||
            cursors_iterator == end_iterator || sharing_link_iterator == end_iterator ||
            !document_iterator->isObject() || !text_iterator->isArray() ||
            !site_ids_iterator->isArray() || !profiles_iterator->isArray() ||
            !cursors_iterator->isArray())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
        site_counter_ = site_counter_iterator->toInt(SharedEditor::invalid_site_counter);
        sharing_link_ = sharing_link_iterator->toString();

        if (sharing_link_.isNull() || site_id_ == SharedEditor::invalid_site_id ||
            site_counter_ == SharedEditor::invalid_site_counter)
            throw std::logic_error("invalid message: invalid fields");

        // text
        QJsonArray json_array = text_iterator->toArray();
        for (const auto& s_json : json_array) {
            if (!s_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            Symbol s(s_json.toObject());
            text_.push_back(s);
        }

        // site_ids
        json_array = site_ids_iterator->toArray();
        for (const auto& u_json : json_array) {
            if (!u_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            QJsonObject u_json_object = u_json.toObject();

            auto end_iterator = u_json_object.end();
            auto username_iterator = u_json_object.find("username");
            auto site_id_iterator = u_json_object.find("site_id");
            if (username_iterator == end_iterator || site_id_iterator == end_iterator)
                throw std::logic_error("invalid message: invalid fields");

            QString username = username_iterator->toString();
            int site_id = site_id_iterator->toInt(SharedEditor::invalid_site_id);
            if (username.isNull() || site_id == SharedEditor::invalid_site_id)
                throw std::logic_error("invalid message: invalid fields");

            site_ids_.insert(username, site_id);
        }

        // profiles
        json_array = profiles_iterator->toArray();
        for (const auto& c_json : json_array) {
            if (!c_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            QJsonObject c_json_object = c_json.toObject();

            auto end_iterator = c_json_object.end();
            auto username_iterator = c_json_object.find("username");
            auto profile_iterator = c_json_object.find("profile");
            if (username_iterator == end_iterator || profile_iterator == end_iterator || !profile_iterator->isObject())
                throw std::logic_error("invalid message: invalid fields");

            QString username = username_iterator->toString();
            Profile profile(profile_iterator->toObject());
            if (username.isNull()) throw std::logic_error("invalid message: invalid fields");

            profiles_.insert(username, profile);
        }

        // cursors
        json_array = cursors_iterator->toArray();
        for (const auto& c_json : json_array) {
            if (!c_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            QJsonObject c_json_object = c_json.toObject();

            auto end_iterator = c_json_object.end();
            auto username_iterator = c_json_object.find("username");
            auto symbol_iterator = c_json_object.find("symbol");
            if (username_iterator == end_iterator || symbol_iterator == end_iterator || !symbol_iterator->isObject())
                throw std::logic_error("invalid message: invalid fields");

            QString username = username_iterator->toString();
            Symbol symbol(symbol_iterator->toObject());
            if (username.isNull()) throw std::logic_error("invalid message: invalid fields");

            cursors_.insert(username, symbol);
        }
    }

    bool DocumentMessage::operator==(const Message& other) const {
        const DocumentMessage *o = dynamic_cast<const DocumentMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->text_ == o->text_ &&
               this->site_id_ == o->site_id_ && this->site_counter_ == o->site_counter_ &&
               this->site_ids_ == o->site_ids_ && this->profiles_ == o->profiles_ &&
               this->cursors_ == o->cursors_ && this->sharing_link_ == o->sharing_link_;
    }

    Document& DocumentMessage::document() {
        return document_;
    }

    QList<Symbol>& DocumentMessage::text() {
        return text_;
    }

    int DocumentMessage::site_id() const {
        return site_id_;
    }

    int DocumentMessage::site_counter() const {
        return site_counter_;
    }

    QHash<QString,int>& DocumentMessage::site_ids() {
        return site_ids_;
    }

    QHash<QString,Profile>& DocumentMessage::profiles() {
        return profiles_;
    }

    QHash<QString,Symbol>& DocumentMessage::cursors() {
        return cursors_;
    }

    QString& DocumentMessage::sharing_link() {
        return sharing_link_;
    }

    QJsonObject DocumentMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["sharing_link"] = sharing_link_;
        json_object["site_id"] = site_id_;
        json_object["site_counter"] = site_counter_;

        // text
        QJsonArray json_array;
        for (const auto& s : text_)
            json_array.push_back(s.json());
        json_object["text"] = json_array;

        // site_ids
        json_array = QJsonArray{};
        for (auto it=site_ids_.begin(); it!=site_ids_.end(); it++) {
            QJsonObject u_json;
            u_json["username"] = it.key();
            u_json["site_id"] = it.value();
            json_array.push_back(u_json);
        }
        json_object["site_ids"] = json_array;

        // profiles
        json_array = QJsonArray{};
        for (auto it=profiles_.begin(); it!=profiles_.end(); it++) {
            QJsonObject c_json;
            c_json["username"] = it.key();
            c_json["profile"] = it.value().json();
            json_array.push_back(c_json);
        }
        json_object["profiles"] = json_array;

        // cursors
        json_array = QJsonArray{};
        for (auto it=cursors_.begin(); it!=cursors_.end(); it++) {
            QJsonObject c_json;
            c_json["username"] = it.key();
            c_json["symbol"] = it.value().json();
            json_array.push_back(c_json);
        }
        json_object["cursors"] = json_array;

        return json_object;
    }
}
