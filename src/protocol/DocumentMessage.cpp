/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>
#include <crdt/SharedEditor.h>
#include <QtCore/QJsonArray>

namespace collaborative_text_editor {
    DocumentMessage::DocumentMessage(const Document& document, const QVector<Symbol>& text,
                                     const QHash<int, Profile>& users, const QHash<QString, Symbol>& cursors,
                                     const QString& sharing_link) :
        Message(MessageType::document), document_(document), text_(text), users_(users), cursors_(cursors),
        sharing_link_(sharing_link) {}

    DocumentMessage::DocumentMessage(const QJsonObject &json_object) : Message(MessageType::document) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto text_iterator = json_object.find("text");
        auto users_iterator = json_object.find("users");
        auto cursors_iterator = json_object.find("cursors");
        auto sharing_link_iterator = json_object.find("sharing_link");

        if (document_iterator == end_iterator || text_iterator == end_iterator ||
            users_iterator == end_iterator || cursors_iterator == end_iterator ||
            sharing_link_iterator == end_iterator || !document_iterator->isObject() ||
            !text_iterator->isArray() || !users_iterator->isArray() || !cursors_iterator->isArray())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        sharing_link_ = sharing_link_iterator->toString();

        if (sharing_link_.isNull())
            throw std::logic_error("invalid message: invalid fields");

        // text
        QJsonArray json_array = text_iterator->toArray();
        for (const auto& s_json : json_array) {
            if (!s_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            Symbol s(s_json.toObject());
            text_.push_back(s);
        }

        // users
        json_array = users_iterator->toArray();
        for (const auto& u_json : json_array) {
            if (!u_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            QJsonObject u_json_object = u_json.toObject();

            auto end_iterator = u_json_object.end();
            auto site_id_iterator = u_json_object.find("site_id");
            auto profile_iterator = u_json_object.find("profile");
            if (site_id_iterator == end_iterator || profile_iterator == end_iterator || !profile_iterator->isObject())
                throw std::logic_error("invalid message: invalid fields");

            int site_id = site_id_iterator->toInt(SharedEditor::invalid_site_id);
            if (site_id == SharedEditor::invalid_site_id)
                throw std::logic_error("invalid message: invalid fields");
            Profile profile(profile_iterator->toObject());

            users_.insert(site_id, profile);
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
            if (username.isNull()) throw std::logic_error("invalid message: invalid fields");
            Symbol symbol(symbol_iterator->toObject());

            cursors_.insert(username, symbol);
        }
    }

    bool DocumentMessage::operator==(const Message& other) const {
        const DocumentMessage *o = dynamic_cast<const DocumentMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->text_ == o->text_ &&
               this->users_ == o->users_ && this->cursors_ == o->cursors_ &&
               this->sharing_link_ == o->sharing_link_;
    }
        
    Document DocumentMessage::document() const {
        return document_;
    }

    QVector<Symbol> DocumentMessage::text() const {
        return text_;
    }

    QHash<int, Profile> DocumentMessage::users() const {
        return users_;
    }

    QHash<QString, Symbol> DocumentMessage::cursors() const {
        return cursors_;
    }

    QString DocumentMessage::sharing_link() const {
        return sharing_link_;
    }

    QJsonObject DocumentMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["sharing_link"] = sharing_link_;

        // text
        QJsonArray json_array;
        for (const auto& s : text_)
            json_array.push_back(s.json());
        json_object["text"] = json_array;

        // users
        json_array = QJsonArray{};
        for (auto it=users_.begin(); it!=users_.end(); it++) {
            QJsonObject u_json;
            u_json["site_id"] = it.key();
            u_json["profile"] = it.value().json();
            json_array.push_back(u_json);
        }
        json_object["users"] = json_array;

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
