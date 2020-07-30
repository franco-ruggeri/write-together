/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>
#include <crdt/SharedEditor.h>
#include <QtCore/QJsonArray>

namespace collaborative_text_editor {
    DocumentMessage::DocumentMessage(const Document& document, const QVector<Symbol>& text,
                                     const QHash<int, Profile>& users, const QString& sharing_link) :
        Message(MessageType::document), document_(document), text_(text), users_(users), sharing_link_(sharing_link) {}

    DocumentMessage::DocumentMessage(const QJsonObject &json_object) : Message(MessageType::document) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto text_iterator = json_object.find("text");
        auto users_iterator = json_object.find("users");
        auto sharing_link_iterator = json_object.find("sharing_link");

        if (document_iterator == end_iterator || text_iterator == end_iterator ||
            users_iterator == end_iterator || sharing_link_iterator == end_iterator ||
            !document_iterator->isObject() || !text_iterator->isArray() || !users_iterator->isArray())
            throw std::logic_error("invalid message: invalid fields");

        document_ = Document(document_iterator->toObject());
        sharing_link_ = sharing_link_iterator->toString();

        if (sharing_link_.isNull())
            throw std::logic_error("invalid message: invalid fields");

        text_ = QVector<Symbol>{};
        QJsonArray text_json = text_iterator->toArray();
        for (const auto& s_json : text_json) {
            if (!s_json.isObject()) throw std::logic_error("invalid message: invalid fields");
            Symbol s(s_json.toObject());
            text_.push_back(s);
        }

        users_ = QHash<int,Profile>{};
        QJsonArray users_json = users_iterator->toArray();
        for (const auto& u_json : users_json) {
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
    }

    bool DocumentMessage::operator==(const Message& other) const {
        const DocumentMessage *o = dynamic_cast<const DocumentMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->text_ == o->text_ &&
               this->users_ == o->users_ && this->sharing_link_ == o->sharing_link_;
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

    QString DocumentMessage::sharing_link() const {
        return sharing_link_;
    }

    QJsonObject DocumentMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["document"] = document_.json();
        json_object["sharing_link"] = sharing_link_;

        QJsonArray json_array;
        for (const auto& s : text_)
            json_array.push_back(s.json());
        json_object["text"] = json_array;

        json_array = QJsonArray{};
        for (auto it=users_.begin(); it!=users_.end(); it++) {
            QJsonObject u_json;
            u_json["site_id"] = it.key();
            u_json["profile"] = it.value().json();
            json_array.push_back(u_json);
        }
        json_object["users"] = json_array;

        return json_object;
    }
}
