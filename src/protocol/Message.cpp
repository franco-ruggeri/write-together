/*
 * Author: Franco Ruggeri
 */

#include <protocol/Message.h>
#include <protocol/ErrorMessage.h>
#include <protocol/SignUpMessage.h>
#include <protocol/LoginMessage.h>
#include <protocol/LogoutMessage.h>
#include <protocol/DocumentsMessage.h>
#include <protocol/CreateMessage.h>
#include <protocol/OpenMessage.h>
#include <protocol/TextMessage.h>
#include <protocol/InsertMessage.h>
#include <protocol/EraseMessage.h>
#include <protocol/CursorMessage.h>
#include <protocol/CloseMessage.h>
#include <protocol/ProfileMessage.h>
#include <protocol/ProfileOkMessage.h>
#include <QJsonDocument>

namespace collaborative_text_editor {
    Message::Message(MessageType type) : type_(type) {}

    MessageType Message::type() const {
        return type_;
    }

    QJsonObject Message::json() const {
        return {{"type", static_cast<int>(type_)}};
    }

    QByteArray Message::serialize() const {
        QJsonObject json_object = json();
        QJsonDocument json_document(json_object);
        QByteArray json_bytes = json_document.toJson(QJsonDocument::JsonFormat::Compact);
        return json_bytes;
    }

    std::shared_ptr<Message> Message::deserialize(const QByteArray& serialized_message) {
        std::shared_ptr<Message> message;

        // get json object
        QJsonDocument json_document = QJsonDocument::fromJson(serialized_message);
        if (json_document.isNull() || !json_document.isObject()) throw std::logic_error("invalid message: not a json object");
        QJsonObject json_object = json_document.object();

        // get type
        auto type_iterator = json_object.find("type");
        if (type_iterator == json_object.end()) throw std::logic_error("invalid message: no type field");
        MessageType type = static_cast<MessageType>(type_iterator.value().toInt());

        // create message
        switch (type) {
            case MessageType::error:
                message = std::shared_ptr<ErrorMessage>(new ErrorMessage(json_object));
                break;
            case MessageType::sign_up:
                message = std::shared_ptr<SignUpMessage>(new SignUpMessage(json_object));
                break;
            case MessageType::login:
                message = std::shared_ptr<LoginMessage>(new LoginMessage(json_object));
                break;
            case MessageType::logout:
                message = std::shared_ptr<LogoutMessage>(new LogoutMessage(json_object));
                break;
            case MessageType::documents:
                message = std::shared_ptr<DocumentsMessage>(new DocumentsMessage(json_object));
                break;
            case MessageType::create:
                message = std::shared_ptr<CreateMessage>(new CreateMessage(json_object));
                break;
            case MessageType::open:
                message = std::shared_ptr<OpenMessage>(new OpenMessage(json_object));
                break;
            case MessageType::text:
                message = std::shared_ptr<TextMessage>(new TextMessage(json_object));
                break;
            case MessageType::insert:
                message = std::shared_ptr<InsertMessage>(new InsertMessage(json_object));
                break;
            case MessageType::erase:
                message = std::shared_ptr<EraseMessage>(new EraseMessage(json_object));
                break;
            case MessageType::cursor:
                message = std::shared_ptr<CursorMessage>(new CursorMessage(json_object));
                break;
            case MessageType::close:
                message = std::shared_ptr<CloseMessage>(new CloseMessage(json_object));
                break;
            case MessageType::profile:
                message = std::shared_ptr<ProfileMessage>(new ProfileMessage(json_object));
                break;
            case MessageType::profile_ok:
                message = std::shared_ptr<ProfileOkMessage>(new ProfileOkMessage(json_object));
                break;
            default:
                throw std::logic_error("invalid message: invalid type");
        }
        return message;
    }
}
