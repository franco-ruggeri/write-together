#include <cte/protocol/open_message.h>
#include <cte/crdt/shared_editor.h>

namespace cte {
    OpenMessage::OpenMessage(const Document& document) : Message(MessageType::open), document_(document) {}

    OpenMessage::OpenMessage(const Document& document, int site_id, const Profile& profile) :
        Message(MessageType::open), document_(document), site_id_(site_id), profile_(profile) {}

    OpenMessage::OpenMessage(const QUrl& sharing_link) : Message(MessageType::open), sharing_link_(sharing_link) {}

    OpenMessage::OpenMessage(const QJsonObject &json_object) : Message(MessageType::open) {
        auto end_iterator = json_object.end();
        auto document_iterator = json_object.find("document");
        auto sharing_link_iterator = json_object.find("sharing_link");
        auto site_id_iterator = json_object.find("site_id");
        auto profile_iterator = json_object.find("profile");

        // check combination of fields
        if ((document_iterator != end_iterator && sharing_link_iterator != end_iterator) ||
            (sharing_link_iterator != end_iterator &&
             (site_id_iterator != end_iterator || profile_iterator != end_iterator)) ||
            (site_id_iterator != end_iterator && profile_iterator == end_iterator) ||
            (site_id_iterator == end_iterator && profile_iterator != end_iterator))
            throw std::logic_error("invalid message: invalid usage");

        if (document_iterator != end_iterator) {
            if (!document_iterator->isObject())
                throw std::logic_error("invalid message: invalid fields");
            document_ = Document(document_iterator->toObject());

            if (site_id_iterator != end_iterator && profile_iterator != end_iterator) {
                site_id_ = site_id_iterator->toInt(SharedEditor::invalid_site_id);
                if (*site_id_ == SharedEditor::invalid_site_id)
                    throw std::logic_error("invalid message: invalid fields");

                if (!profile_iterator->isObject())
                    throw std::logic_error("invalid message: invalid fields");
                profile_ = Profile(profile_iterator->toObject());
            }
        } else {
            QString sharing_link = sharing_link_iterator->toString();
            if (sharing_link.isNull())
                throw std::logic_error("invalid message: invalid fields");
            sharing_link_ = sharing_link;
        }
    }

    bool OpenMessage::operator==(const Message& other) const {
        const auto *o = dynamic_cast<const OpenMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->document_ == o->document_ && this->sharing_link_ == o->sharing_link_ &&
               this->site_id_ == o->site_id_ && this->profile_ == o->profile_;
    }

    std::optional<Document> OpenMessage::document() const {
        return document_;
    }

    std::optional<QUrl> OpenMessage::sharing_link() const {
        return sharing_link_;
    }

    std::optional<int> OpenMessage::site_id() const {
        return site_id_;
    }

    std::optional<Profile> OpenMessage::profile() const {
        return profile_;
    }

    QJsonObject OpenMessage::json() const {
        QJsonObject json_object = Message::json();
        if (document_) json_object["document"] = document_->json();
        if (sharing_link_) json_object["sharing_link"] = sharing_link_->toString();
        if (site_id_) json_object["site_id"] = *site_id_;
        if (profile_) json_object["profile"] = profile_->json();
        return json_object;
    }
}