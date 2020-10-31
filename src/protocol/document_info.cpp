#include <cte/protocol/document_info.h>
#include <cte/crdt/shared_editor.h>
#include <QtCore/QJsonArray>
#include <QDebug>

namespace cte {
    DocumentInfo::DocumentInfo() : site_id_(SharedEditor::invalid_site_id) {}

    DocumentInfo::DocumentInfo(int site_id_user, const QUrl& sharing_link) :
        site_id_(site_id_user), sharing_link_(sharing_link) {}

    DocumentInfo::DocumentInfo(const QList<std::pair<Symbol,Format>>& text, int site_id,
                               const QHash<int,Symbol>& cursors,
                               const QHash<QString,std::pair<Profile,QList<int>>>& users, const QUrl& sharing_link) :
        text_(text), site_id_(site_id), cursors_(cursors), users_(users), sharing_link_(sharing_link) {}

   DocumentInfo::DocumentInfo(const QJsonObject& json_object) {
       auto end_iterator = json_object.end();
       auto text_iterator = json_object.find("text");
       auto site_id_iterator = json_object.find("site_id");
       auto cursors_iterator = json_object.find("cursors");
       auto users_iterator = json_object.find("users");
       auto sharing_link_iterator = json_object.find("sharing_link");

       if (text_iterator == end_iterator || site_id_iterator == end_iterator ||
           cursors_iterator == end_iterator || users_iterator == end_iterator ||
           sharing_link_iterator == end_iterator || !text_iterator->isArray() ||
           !cursors_iterator->isArray() || !users_iterator->isArray())
           throw std::logic_error("invalid message: invalid fields");

       site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
       QString sharing_link = sharing_link_iterator->toString();
       if (site_id_ == SharedEditor::invalid_site_id || sharing_link.isNull())
           throw std::logic_error("invalid message: invalid fields");
       sharing_link_ = QUrl(sharing_link);

       // text
       QJsonArray json_array = text_iterator->toArray();
       for (const auto &s_json : json_array) {
           if (!s_json.isObject()) throw std::logic_error("invalid message: invalid fields");
           QJsonObject s_json_object = s_json.toObject();

           auto end_iterator = s_json_object.end();
           auto symbol_iterator = s_json_object.find("symbol");
           auto format_iterator = s_json_object.find("format");

           if (symbol_iterator == end_iterator || format_iterator == end_iterator ||
                !symbol_iterator->isObject() || !format_iterator->isObject())
               throw std::logic_error("invalid message: invalid fields");

           Symbol s(symbol_iterator->toObject());
           Format f(format_iterator->toObject());
           text_.push_back({s, f});
       }

       // cursors
       json_array = cursors_iterator->toArray();
       for (const auto &c_json : json_array) {
           if (!c_json.isObject()) throw std::logic_error("invalid message: invalid fields");
           QJsonObject c_json_object = c_json.toObject();

           auto end_iterator = c_json_object.end();
           auto site_id_iterator = c_json_object.find("site_id");
           auto symbol_iterator = c_json_object.find("symbol");
           if (site_id_iterator == end_iterator || symbol_iterator == end_iterator || !symbol_iterator->isObject())
               throw std::logic_error("invalid message: invalid fields");

           int site_id = site_id_iterator->toInt(SharedEditor::invalid_site_id);
           Symbol symbol(symbol_iterator->toObject());
           if (site_id == SharedEditor::invalid_site_id) throw std::logic_error("invalid message: invalid fields");

           cursors_.insert(site_id, symbol);
       }

       // users
       json_array = users_iterator->toArray();
       for (const auto& u_json : json_array) {
           if (!u_json.isObject()) throw std::logic_error("invalid message: invalid fields");
           QJsonObject u_json_object = u_json.toObject();

           auto end_iterator = u_json_object.end();
           auto username_iterator = u_json_object.find("username");
           auto profile_iterator = u_json_object.find("profile");
           auto site_ids_iterator = u_json_object.find("site_ids");
           if (username_iterator == end_iterator || profile_iterator == end_iterator ||
                site_ids_iterator == end_iterator || !site_ids_iterator->isArray())
               throw std::logic_error("invalid message: invalid fields");

           QString username = username_iterator->toString();
           Profile profile(profile_iterator->toObject());

           QList<int> site_ids;
           QJsonArray si_json_array = site_ids_iterator->toArray();
           for (const auto& si_json : si_json_array) {
               int site_id = si_json.toInt(SharedEditor::invalid_site_id);
               if (site_id == SharedEditor::invalid_site_id)
                   throw std::logic_error("invalid message: invalid fields");
               site_ids.append(site_id);
           }

           users_.insert(username, {profile, site_ids});
       }
   }

   bool DocumentInfo::operator==(const DocumentInfo& other) const {
       return this->text_ == other.text_ && this->site_id_ == other.site_id_ &&
              this->cursors_ == other.cursors_ && this->users_ == other.users_ &&
              this->sharing_link_ == other.sharing_link_;
    }

   QList<std::pair<Symbol,Format>> DocumentInfo::text() const {
        return text_;
    }

    int DocumentInfo::site_id() const {
        return site_id_;
    }

    QHash<int,Symbol> DocumentInfo::cursors() const {
        return cursors_;
    }

    QHash<QString,std::pair<Profile,QList<int>>> DocumentInfo::users() const {
        return users_;
    }

    QUrl DocumentInfo::sharing_link() const {
        return sharing_link_;
    }

    QJsonObject DocumentInfo::json() const {
        QJsonObject json_object;

        json_object["sharing_link"] = sharing_link().toString();
        json_object["site_id"] = site_id_;

        // text
        QJsonArray json_array;
        for (auto it=text_.begin(); it!=text_.end(); it++) {
            QJsonObject s_json;
            s_json["symbol"] = it->first.json();
            s_json["format"] = it->second.json();
            json_array.push_back(s_json);
        }
        json_object["text"] = json_array;

        // cursors
        json_array = QJsonArray{};
        for (auto it=cursors_.begin(); it!=cursors_.end(); it++) {
            QJsonObject c_json;
            c_json["site_id"] = it.key();
            c_json["symbol"] = it.value().json();
            json_array.push_back(c_json);
        }
        json_object["cursors"] = json_array;

        // users
        json_array = QJsonArray{};
        for (auto it=users_.begin(); it!=users_.end(); it++) {
            QJsonObject u_json;
            u_json["username"] = it.key();
            u_json["profile"] = it.value().first.json();
            QJsonArray si_json;
            for (int si : it.value().second)
                si_json.push_back(si);
            u_json["site_ids"] = si_json;
            json_array.push_back(u_json);
        }
        json_object["users"] = json_array;

        return json_object;
    }
}
