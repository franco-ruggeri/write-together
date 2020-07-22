/*
 * Author: Franco Ruggeri
 */

#include "ProfileMessage.h"
#include <QBuffer>
#include <QIODevice>

namespace collaborative_text_editor {
    ProfileMessage::ProfileMessage(const QString& username, const QString& password, const QImage& icon) :
        Message(MessageType::profile), username_(username), password_(password), icon_(icon) {}

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
