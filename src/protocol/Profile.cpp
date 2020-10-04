/*
 * Author: Franco Ruggeri, Antonino Musmeci
 */

#include <cte/protocol/Profile.h>
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>

namespace cte {
    static QRegExp username_regexp("^[a-zA-Z0-9]{3,}$");
    static QRegExp email_regexp("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}$");
    static QRegExp password_regexp("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$");

    Profile::Profile() {}

    Profile::Profile(const QString &username, const QString &name, const QString &surname, const QString& email,
                     const QImage& icon) :
        username_(username), name_(name), surname_(surname), email_(email), icon_(icon) {}

    Profile::Profile(const QJsonObject &json_object) {
        auto end_iterator = json_object.end();
        auto username_iterator = json_object.find("username");
        auto name_iterator = json_object.find("name");
        auto surname_iterator = json_object.find("surname");
        auto email_iterator = json_object.find("email");
        auto icon_iterator = json_object.find("icon");

        if (username_iterator == end_iterator || name_iterator == end_iterator ||
            surname_iterator == end_iterator || email_iterator == end_iterator ||
            icon_iterator == end_iterator)
            throw std::logic_error("invalid message: invalid fields");

        username_ = username_iterator->toString();
        name_ = name_iterator->toString();
        surname_ = surname_iterator->toString();
        email_ = email_iterator->toString();
        QString icon_base64 = icon_iterator->toString();

        if (username_.isNull() || name_.isNull() || surname_.isNull() || email_.isNull() || icon_base64.isNull())
            throw std::logic_error("invalid message: invalid fields");

        QByteArray bytes = QByteArray::fromBase64(icon_base64.toLatin1());
        icon_.loadFromData(bytes, "PNG");
    }

    bool Profile::operator==(const Profile& other) const {
        return this->username_ == other.username_ && this->name_ == other.name_ && this->surname_ == other.surname_ &&
               this->email_ == other.email_ && this->icon_ == other.icon_;
    }

    QString Profile::username() const {
        return username_;
    }

    QString Profile::name() const {
        return name_;
    }

    QString Profile::surname() const {
        return surname_;
    }

    QString Profile::email() const {
        return email_;
    }

    QImage Profile::icon() const {
        return icon_;
    }

    QJsonObject Profile::json() const {
        QJsonObject json_object;

        json_object["username"] = username_;
        json_object["name"] = name_;
        json_object["surname"] = surname_;
        json_object["email"] = email_;

        // icon in base64
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        icon_.save(&buffer, "PNG");
        json_object["icon"] = QLatin1String(bytes.toBase64());

        return json_object;
    }

    bool Profile::check_username(const QString& username) {
        return username_regexp.exactMatch(username);
    }

    bool Profile::check_email(const QString& email) {
        return email_regexp.exactMatch(email);
    }

    bool Profile::check_password(const QString& password) {
        return password_regexp.exactMatch(password);
    }
}
