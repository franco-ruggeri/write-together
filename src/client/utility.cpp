
#include "client/utility.h"

bool utility::check_ip_address(const QString& address) {
    return QRegExp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)[.]){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$").exactMatch(address);
}

bool utility::check_email_validity(const QString &email) {
    return QRegExp("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}$").exactMatch(email);;
}
bool utility::check_username_validity(const QString &username) {
    return QRegExp("^[a-zA-Z0-9]{3,}$").exactMatch(username);
}

bool utility::check_password_validity(const QString &password) {
    return QRegExp("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$").exactMatch(password);
}


