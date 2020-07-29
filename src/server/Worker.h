/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QSharedPointer>
#include <network/TcpSocket.h>
#include "User.h"

using namespace collaborative_text_editor;

class Worker : public QObject {
    Q_OBJECT

    QHash<int,User> users_;     // socket_fd -> user
    QMutex m_users_;            // access both from main thread (new connections) and worker thread (event loop)

    void signup(User& user, QSharedPointer<Message> message);
    void login(User& user, QSharedPointer<Message> message);
    void logout(User& user, QSharedPointer<Message> message);
    void create_document(User& user, QSharedPointer<Message> message);
    void open_document(User& user, QSharedPointer<Message> message);
    void insert_symbol(User& user, QSharedPointer<Message> message);
    void erase_symbol(User& user, QSharedPointer<Message> message);
    void move_cursor(User &user, QSharedPointer<Message> message);
    void close_document(User& user, QSharedPointer<Message> message);
    void update_profile(User& user, QSharedPointer<Message> message);

signals:
    void new_connection(int socket_fd);
    void new_message(QSharedPointer<Message> message);

private slots:
    void serve_request();

public slots:
    void create_socket(int socket_fd);
    void dispatch_message(QSharedPointer<Message> message);

public:
    void assign_connection(qintptr socket_fd);
    unsigned int number_of_connections();
};
