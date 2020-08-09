/*
 * Author: Antonino Musmeci
 */


#pragma once

#include <optional>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtNetwork/QTcpSocket>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/Profile.h>
#include <cte/crdt/Symbol.h>
#include <cte/client/utility.h>
#include <cte/client/fileInfo.h>
#include <cte/client/UserInfo.h>
#define PORT 1111

using namespace cte;

class myClient : public QObject {

public:
    QTcpSocket *socket;

//    Profile user;
    UserInfo user;

    myClient(QObject *parent = nullptr);

    bool connect(QString ip_address);

    std::tuple<bool, QString> login(QString &email, QString &password);

    QSharedPointer<Message> send_message(const QSharedPointer<Message>& request);

    std::tuple<bool, QString> signup(QString& username, QString& email, QString& password);

    void logout();


    std::optional<fileInfo> new_file(const QString& filename);

    bool change_password(const QString& new_password);

    void sendErase(int pos);

    fileInfo
    open_file(const QString& filename);


    void file_close(const fileInfo& file);


    bool change_username(const QString &new_username);

    std::optional<QString> get_uri(const QString &filename);

    QSet<Document> get_documents_form_server();

    void sendErase(const Document &document, const Symbol &s);

    void sendInsert(const Document &document, const Symbol &s);

    void send_cursor(Document document, Symbol cursor_position);

    std::tuple<bool, QString>
    signup(QString &username, QString &email, QString &password, QString name, QString surname);
};


