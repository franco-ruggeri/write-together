/*
 * Server worker. It can manage many sessions at the same time. A session represents a network connection (a possible
 * identifier is the socket file descriptor).
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QMutex>
#include <cte/network/socket.h>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>

namespace cte {
    class Worker : public QObject {
        Q_OBJECT

        QHash<Document,QSet<Socket*>> editing_clients;     // for dispatching
        int number_of_connections_;
        mutable QMutex m_number_of_connections_;

        // identity management
        void signup(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void login(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void logout(int session_id, Socket *socket);
        void update_profile(int session_id, Socket *socket, const QSharedPointer<Message>& message);

        // document management
        void create_document(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void open_document(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void close_document(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void get_document_list(int session_id, Socket *socket, const QSharedPointer<Message>& message);

        // document editing
        void insert_symbol(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void erase_symbol(int session_id, Socket *socket, const QSharedPointer<Message>& message);
        void move_cursor(int session_id, Socket *socket, const QSharedPointer<Message>& message);

    signals:
        void new_connection(int socket_fd);
        void new_message(int source_socket_fd, const QSharedPointer<Message>& message);

    private slots:
        void start_session(int socket_fd);
        void close_session(int session_id, Socket *socket);
        void serve_request(int session_id, Socket *socket);
        void dispatch_message(int source_socket_fd, const QSharedPointer<Message>& message);

    public:
        Worker(QObject *parent=nullptr);
        void assign_connection(int socket_fd);
        unsigned int number_of_connections() const;
        static void connect(const Worker& worker1, const Worker& worker2);
    };
}
