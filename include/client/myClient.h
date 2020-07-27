/*
 * Author: Antonino Musmeci
 */


#pragma once

#include <QJsonObject>
#include <QTcpSocket>
#include <QString>
#include "client/utility.h"
#include "protocol/Message.h"
#include "crdt/Symbol.h"
#include "protocol/User.h"
#include "fileInfo.h"
#include <optional>
#define PORT 1111

using namespace collaborative_text_editor;
class myClient : public QObject {

public:
    QTcpSocket *socket;

    User *user;

    myClient(QObject *parent = nullptr);

    bool connect(QString ip_address);

    std::tuple<bool,std::vector<QString>> login(QString &email, QString &password);

    std::shared_ptr<Message> send_message(const std::shared_ptr<Message>& request);

    std::tuple<bool,std::vector<QString>> signup(QString& username, QString& email, QString& password);

    void logout();

    void sendInsert(const QString& filename, const Symbol& s);

    std::tuple<bool, QString> new_file(const QString& filename);

    bool change_password(const QString& new_password);

    void sendErase(const QString& filename, const Symbol& s);

    void sendErase(int pos);

    std::tuple<bool,QString> open_file(const QString& filename);


    void file_close(const fileInfo& file);


    bool change_username(const QString &new_username);

    std::optional<QString> get_uri(const QString &filename);
};


