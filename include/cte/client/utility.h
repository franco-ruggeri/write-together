/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <QtCore/QStringList>
#include <QtCore/QJsonObject>

//using namespace  collaborative_text_editor;
class utility {

public:
    static bool check_ip_address(const QString& address);
    static bool check_email_validity(const QString& email);
    static bool check_username_validity(const QString& username);
    static bool check_password_validity(const QString& password);


};



