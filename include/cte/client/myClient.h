/*
 * Author: Antonino Musmeci, Stefano Di Blasio
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

using namespace cte;

class myClient : public QObject {
    Q_OBJECT
    QSharedPointer<QTimer> wait_on_connection_, connecting_interrupt_; // the second for qt backward compatibility
    QSharedPointer<Message> message_to_send_;
    Profile new_user;
    QString host_address_, fallback_host_address_, host_to_connect_;
    quint16 port_;
    bool ssl_handshake_failed_;
    // only for qt backward compatibility
    QAbstractSocket::SocketState previous_state_;

    void send_message(const QSharedPointer<Message>& request);

public:
    Socket *socket;

    UserInfo user;

    myClient(QObject *parent = nullptr);
    virtual ~myClient() noexcept;

    void destroy_previous_connection(); // in case the server disconnects

    void login(QString &email, QString &password);

    void logout();


    void new_file(const QString& filename);

    void open_file(const QString& file, bool isFilename = true);
    void file_close(const fileInfo& file);

    void get_documents_form_server();

    void sendErase(const Document &document, const Symbol &s);

    void sendInsert(const Document &document, const Symbol &s);

    void send_cursor(Document document, Symbol cursor_position);

    void signup(QString &username, QString &email, QString &password, QString name, QString surname, QImage icon = QImage());
    void update_profile(const QString& username, const QString& email, const QString& name, const QString& surname, const QImage& icon, QString& password);

public slots:
    void connect(const QString& ip_address = "localhost", quint16 ip_port = 1111);

private slots:
    void handle_connection_error(QAbstractSocket::SocketError error); // mainly for server unreachable
    void handle_changed_state(QAbstractSocket::SocketState new_state);
    void timeout_on_connection();
    void handle_ssl_handshake(const QList<QSslError>& errors); // error in verifying the peer
    void connection_encrypted(); // the connection is established and encrypted
    void attempt_timeout(); // to retry if less than maximum attempts or to signal timeout error
    void process_response(); // elaborate server message (response)
    void process_data_from_server(); // process any "interactive" message from the server

signals:
    void generic_error(const QString& error);
    void host_connected(bool result);
    void server_disconnected();
    void timeout_expired(const QString& message_type);
    void authentication_result(bool authenticated, const QString& error_message);
    void profile_update_result(bool authenticated, const QString& error_message);
    void user_documents(const QSet<Document>& documents);
    void document(fileInfo& file);

    void user_added(const Profile& user_profile, int site_id);
    void user_removed(const QString& username, int site_id);
    void char_inserted(const Symbol& symbol);
    void char_removed(const Symbol& symbol);
    void cursor(const Symbol& symbol, const QString& username);
};


