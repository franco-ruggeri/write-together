/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileMessage.h>
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>

namespace collaborative_text_editor {
    ProfileMessage::ProfileMessage(const QString& username, const QString& password, const QImage& icon) :
        Message(MessageType::profile), username_(username), password_(password), icon_(icon) {}

    ProfileMessage::ProfileMessage(const QJsonObject &json_object) : Message(MessageType::profile) {
        auto end_iterator = json_object.end();
        auto username_iterator = json_object.find("username");
        auto password_iterator = json_object.find("password");
        auto icon_iterator = json_object.find("icon");

        if (username_iterator == end_iterator || password_iterator == end_iterator || icon_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        username_ = username_iterator->toString();
        password_ = password_iterator->toString();
        QString icon_base64 = icon_iterator->toString();

        if (username_.isNull() || password_.isNull() || icon_base64.isNull())
            throw std::logic_error("invalid message: invalid fields");

        QByteArray bytes = QByteArray::fromBase64(icon_base64.toLatin1());
        icon_.loadFromData(bytes, "PNG");
    }

    bool ProfileMessage::operator==(const Message& other) const {
        const ProfileMessage *o = dynamic_cast<const ProfileMessage*>(&other);
        return o != nullptr && this->type() == o->type() &&
               this->username_ == o->username_ && this->password_ == o->password_ && this->icon_ == o->icon_;
    }
        
    QString ProfileMessage::username() const {
        return username_;
    }

    QString ProfileMessage::password() const {
        return password_;
    }

    QImage ProfileMessage::icon() const {
        return icon_;
    }

    QJsonObject ProfileMessage::json() const {
        QJsonObject json_object = Message::json();
        json_object["username"] = username_;
        json_object["password"] = password_;

        // icon in base64
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        icon_.save(&buffer, "PNG");
        json_object["icon"] = QLatin1String(bytes.toBase64());

        return json_object;
    }
}
