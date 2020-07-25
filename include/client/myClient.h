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
#define PORT 1111

using namespace collaborative_text_editor;
class myClient : public QObject {

public:
    QTcpSocket *socket;

    myClient(QObject *parent = nullptr);

    bool connect(QString ip_address);

    std::tuple<bool,std::vector<QString>> login(QString &email, QString &password);

    std::shared_ptr<Message> send_message(const std::shared_ptr<Message>& request);

    std::tuple<bool,std::vector<QString>> signup(QString& username, QString& email, QString& password);

    void logout();

    void sendInsert(QString filename, Symbol s);

    void new_file(QString filename);

    bool change_password(QString new_password);

    void sendErase(QString filename, Symbol s);

    void sendErase(int pos);

    std::tuple<bool,QString> open_file(QString filename);



};


