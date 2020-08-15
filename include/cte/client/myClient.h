/*
 * Author: Antonino Musmeci
 */


#pragma once

#include <optional>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/Profile.h>
#include <cte/crdt/Symbol.h>
#include <cte/client/utility.h>
#include <cte/client/fileInfo.h>
#include <cte/client/UserInfo.h>
#include <cte/network/Socket.h>
#define PORT 1111


using namespace cte;

class myClient : public QObject {
    Q_OBJECT
    QSharedPointer<QTimer> wait_on_connection_;
    int connection_attempts_;
    QSharedPointer<Message> message_to_send_;
    UserInfo new_user;

public:
    Socket *socket;

//    Profile user;
    UserInfo user;

    myClient(QObject *parent = nullptr);

    bool connect(QString ip_address);

    void login(QString &email, QString &password);

    void send_message(const QSharedPointer<Message>& request);

//    std::tuple<bool, QString> signup(QString& username, QString& email, QString& password);

    void logout();


    void new_file(const QString& filename);

    bool change_password(const QString& new_password);

    void sendErase(int pos);

    void open_file(const QString& filename);


    void file_close(const fileInfo& file);


    bool change_username(const QString &new_username);

    std::optional<QString> get_uri(const QString &filename);

    void get_documents_form_server();

    void sendErase(const Document &document, const Symbol &s);

    void sendInsert(const Document &document, const Symbol &s);

    void send_cursor(Document document, Symbol cursor_position);

    void signup(QString &username, QString &email, QString &password, QString name, QString surname);

private slots:
    void attempt_timeout(); // to retry if less than maximum attempts or to signal timeout error
    void process_response(); // elaborate server message (response)
    void process_data_from_server(); // process any "interactive" message from the server

signals:
    void generic_error(const QString& error);
    void authentication_result(bool authenticated, const QString& error_message);
    void profile_update_result(bool authenticated, const QString& error_message);
    void user_documents(const QSet<Document>& documents);
    void document(fileInfo& file);
};


