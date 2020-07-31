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
#include <protocol/Document.h>
#include <protocol/Profile.h>
#define PORT 1111

using namespace collaborative_text_editor;
class myClient : public QObject {

public:
    QTcpSocket *socket;

    Profile user;

    myClient(QObject *parent = nullptr);

    bool connect(QString ip_address);

    std::tuple<bool, QString> login(QString &email, QString &password);

    QSharedPointer<Message> send_message(const QSharedPointer<Message>& request);

    std::tuple<bool, QString> signup(QString& username, QString& email, QString& password);

    void logout();


    std::tuple<bool, QString> new_file(const QString& filename);

    bool change_password(const QString& new_password);

    void sendErase(int pos);

    std::tuple<bool, QHash<QString, Profile>, QVector<Symbol>, QHash<QString, Symbol>> open_file(const QString& filename);


    void file_close(const fileInfo& file);


    bool change_username(const QString &new_username);

    std::optional<QString> get_uri(const QString &filename);

    QList<Document> get_documents_form_server();

    void sendErase(const Document &document, const Symbol &s);

    void sendInsert(const Document &document, const Symbol &s);
};

