/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <network/TcpSocket.h>
#include <protocol/Document.h>
#include "OpenDocument.h"
#include "User.h"

using namespace collaborative_text_editor;

extern QHash<QString,QSharedPointer<Profile>> online_users;
extern QHash<Document,OpenDocument> open_documents;
extern QMutex m_users, m_documents;

class Worker : public QObject {
    Q_OBJECT

    QHash<TcpSocket*,User> users_;
    QHash<Document,QSet<TcpSocket*>> editing_clients_;
    QMutex m_users_;    // just because Server (in main thread) uses number_of_connections()

    // identity management
    void signup(User& user, const QSharedPointer<Message>& message);
    void login(User& user, const QSharedPointer<Message>& message);
    void logout(User& user, const QSharedPointer<Message>& message);
    void update_profile(User& user, const QSharedPointer<Message>& message);

    // document management
    void create_document(User& user, const QSharedPointer<Message>& message);
    void open_document(User& user, const QSharedPointer<Message>& message);
    void close_document(User& user, const QSharedPointer<Message>& message);

    // document editing
    void edit_document(const User &user, const Document &document, const Symbol &symbol,
                       const QSharedPointer<Message> &message_to_dispatch,
                       const std::function<void(const QSharedPointer<SafeSharedEditor> &, const Symbol &)> &edit);
    void insert_symbol(const User& user, const QSharedPointer<Message>& message);
    void erase_symbol(const User& user, const QSharedPointer<Message>& message);
    void move_cursor(const User& user, const QSharedPointer<Message>& message);

signals:
    void new_connection(int socket_fd);
    void new_message(QSharedPointer<Message> message);

private slots:
    void create_socket(int socket_fd);
    void delete_socket(TcpSocket* socket);
    void serve_request(User& user);

public slots:
    void dispatch_message(QSharedPointer<Message> message);

public:
    Worker();
    void assign_connection(int socket_fd);
    unsigned int number_of_connections() const;
};
