#include <cte/protocol/profile.h>
#include <QtCore/QBuffer>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>

namespace cte {
    const char *Profile::icon_format_ = "PNG";

    Profile::Profile() {}

    Profile::Profile(const QString &username, const QString &name, const QString &surname, const QString& email,
                     const QImage& icon) :
            username_(username), name_(name), surname_(surname), email_(email) {
        set_icon(icon);
    }

    Profile::Profile(const QString &username, const QString &name, const QString &surname, const QString& email,
                     const QByteArray& icon) :
            username_(username), name_(name), surname_(surname), email_(email) {
        set_icon(icon);
    }

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

        set_icon(QByteArray::fromBase64(icon_base64.toLatin1()));
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

    QByteArray Profile::icon_data() const {
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        icon_.save(&buffer, icon_format_);
        return bytes;
    }

    void Profile::set_icon(const QImage& icon) {
        static const int width = 96;
        static const int height = 96;
        icon_ = icon.scaled(width, height);
    }

    void Profile::set_icon(const QByteArray& data) {
        QImage icon;
        icon.loadFromData(data, icon_format_);
        set_icon(icon);
    }

    QJsonObject Profile::json() const {
        QJsonObject json_object;
        json_object["username"] = username_;
        json_object["name"] = name_;
        json_object["surname"] = surname_;
        json_object["email"] = email_;
        json_object["icon"] = QLatin1String(icon_data().toBase64());
        return json_object;
    }

    bool Profile::valid_username() const {
        return valid_username(username_);
    }

    bool Profile::valid_username(const QString& username) {
        static QRegExp username_regexp("^[a-zA-Z0-9][a-zA-Z0-9_-]*$");
        return username_regexp.exactMatch(username);
    }

    bool Profile::valid_password(const QString& password) {
        static QRegExp password_regexp("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$");
        return password_regexp.exactMatch(password);
    }

    bool Profile::valid_email() const {
        static QRegExp email_regexp("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}$");
        return email_regexp.exactMatch(email_);
    }

    bool Profile::valid_name() const {
        return !name_.isEmpty();
    }

    bool Profile::valid_surname() const {
        return !surname_.isEmpty();
    }
}
