/*
 * Server worker. It can manage many sessions at the same time. A session represents a network connection (a possible
 * identifier is the socket file descriptor).
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <cte/network/TcpSocket.h>
#include <cte/protocol/Message.h>
#include <cte/protocol/Document.h>
#include <IdentityManager.h>
#include <DocumentManager.h>

class Worker : public QObject {
    Q_OBJECT

    QHash<cte::Document,QSet<cte::TcpSocket*>> editing_clients;     // for dispatching
    int number_of_connections_;
    mutable QMutex m_number_of_connections_;

    // identity management
    void signup(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void login(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void logout(int session_id);
    void update_profile(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);

    // document management
    void create_document(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void open_document(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void close_document(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void accessible_documents(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);

    // document editing
    void insert_symbol(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void erase_symbol(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);
    void move_cursor(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message);

signals:
    void new_connection(int socket_fd);
    void new_message(int source_socket_fd, QSharedPointer<cte::Message> message);

private slots:
    void start_session(int socket_fd);
    void close_session(int session_id, cte::TcpSocket *socket);
    void serve_request(int session_id, cte::TcpSocket *socket);

public slots:
    void dispatch_message(int source_socket_fd, QSharedPointer<cte::Message> message);

public:
    Worker();
    void assign_connection(int socket_fd);
    unsigned int number_of_connections() const;
};
