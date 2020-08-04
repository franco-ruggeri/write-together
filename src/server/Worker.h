/*
 * Server worker. It can manage many sessions at the same time. A session represents a network connection (a possible
 * identifier is the socket file descriptor).
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <editor/network/TcpSocket.h>
#include <editor/protocol/Message.h>
#include <editor/protocol/Document.h>

class Worker : public QObject {
    Q_OBJECT

    QHash<editor::Document,QVector<editor::TcpSocket*>> editing_clients;     // for dispatching
    int number_of_connections_;
    mutable QMutex m_number_of_connections_;

    // identity management
    void signup(int session_id, editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
    void login(int session_id, editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
    void logout(int session_id);
    void update_profile(int session_id, editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);

    // document management
    void create_document(int session_id, editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
//    void open_document(User& user, const QSharedPointer<Message>& message);
//    void close_document(User& user, const QSharedPointer<Message>& message);
//
    // document editing
//    void edit_document(const User &user, const Document &document, const Symbol &symbol,
//                       const QSharedPointer<Message> &message_to_dispatch,
//                       const std::function<void(const QSharedPointer<SafeSharedEditor> &, const Symbol &)> &edit);
//    void insert_symbol(const User& user, const QSharedPointer<Message>& message);
//    void erase_symbol(const User& user, const QSharedPointer<Message>& message);
//    void move_cursor(const User& user, const QSharedPointer<Message>& message);

signals:
    void new_connection(int socket_fd);
    void new_message(int source_socket_fd, QSharedPointer<editor::Message> message);

private slots:
    void start_session(int socket_fd);
    void close_session(int session_id, editor::TcpSocket *socket);
    void serve_request(int session_id, editor::TcpSocket *socket);

public slots:
    void dispatch_message(int source_socket_fd, QSharedPointer<editor::Message> message);

public:
    Worker();
    void assign_connection(int socket_fd);
    unsigned int number_of_connections() const;
};
