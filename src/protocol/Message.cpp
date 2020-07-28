/*
 * Author: Franco Ruggeri
 */

#include <protocol/Message.h>
#include <protocol/ErrorMessage.h>
#include <protocol/SignupMessage.h>
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
#include <QtCore/QJsonDocument>

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

    QSharedPointer<Message> Message::deserialize(const QByteArray& serialized_message) {
        QSharedPointer<Message> message;

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
                message = QSharedPointer<ErrorMessage>(new ErrorMessage(json_object));
                break;
            case MessageType::signup:
                message = QSharedPointer<SignupMessage>(new SignupMessage(json_object));
                break;
            case MessageType::login:
                message = QSharedPointer<LoginMessage>(new LoginMessage(json_object));
                break;
            case MessageType::logout:
                message = QSharedPointer<LogoutMessage>(new LogoutMessage(json_object));
                break;
            case MessageType::documents:
                message = QSharedPointer<DocumentsMessage>(new DocumentsMessage(json_object));
                break;
            case MessageType::create:
                message = QSharedPointer<CreateMessage>(new CreateMessage(json_object));
                break;
            case MessageType::open:
                message = QSharedPointer<OpenMessage>(new OpenMessage(json_object));
                break;
            case MessageType::text:
                message = QSharedPointer<TextMessage>(new TextMessage(json_object));
                break;
            case MessageType::insert:
                message = QSharedPointer<InsertMessage>(new InsertMessage(json_object));
                break;
            case MessageType::erase:
                message = QSharedPointer<EraseMessage>(new EraseMessage(json_object));
                break;
            case MessageType::cursor:
                message = QSharedPointer<CursorMessage>(new CursorMessage(json_object));
                break;
            case MessageType::close:
                message = QSharedPointer<CloseMessage>(new CloseMessage(json_object));
                break;
            case MessageType::profile:
                message = QSharedPointer<ProfileMessage>(new ProfileMessage(json_object));
                break;
            case MessageType::profile_ok:
                message = QSharedPointer<ProfileOkMessage>(new ProfileOkMessage(json_object));
                break;
            default:
                throw std::logic_error("invalid message: invalid type");
        }

        return message;
    }
}
