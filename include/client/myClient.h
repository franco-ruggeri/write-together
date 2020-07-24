//
// Created by Nino on 21/07/2020.
//

#ifndef TEXTEDITORPROJECTS_MYCLIENT_H
#define TEXTEDITORPROJECTS_MYCLIENT_H

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
    myClient(QObject *parent = nullptr);
    bool connect(QString ip_address);

    std::tuple<bool,std::vector<QString>> login(QString &email, QString &password);

    std::shared_ptr<Message> send_message(const std::shared_ptr<Message>& request);

    bool signup(QString &username, QString &email, QString &password);

    void logout();

    void sendInsert(QString filename, Symbol s);

    void new_file(QString filename);

    void sendErase(QString filename, Symbol s);

    void sendErase(int pos);

    std::tuple<bool,QString> open_file(QString filename);

    QTcpSocket *socket;
};


#endif //TEXTEDITORPROJECTS_MYCLIENT_H
