/*
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
#include "IdentityManager.h"
#include "DocumentManager.h"

class Worker : public QObject {
    Q_OBJECT

    int number_of_connections_;
    QHash<QTcpSocket*,QString> users_;
    mutable QMutex m_number_of_connections_;

    // identity management
    void signup(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
    void login(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
    void logout(editor::TcpSocket *socket);
    void update_profile(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);

    // document management
    void create_document(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message);
//    void open_document(User& user, const QSharedPointer<Message>& message);
//    void close_document(User& user, const QSharedPointer<Message>& message);
//
//    // document editing
//    void edit_document(const User &user, const Document &document, const Symbol &symbol,
//                       const QSharedPointer<Message> &message_to_dispatch,
//                       const std::function<void(const QSharedPointer<SafeSharedEditor> &, const Symbol &)> &edit);
//    void insert_symbol(const User& user, const QSharedPointer<Message>& message);
//    void erase_symbol(const User& user, const QSharedPointer<Message>& message);
//    void move_cursor(const User& user, const QSharedPointer<Message>& message);

signals:
    void new_connection(int socket_fd);
    void new_message(QSharedPointer<editor::Message> message);

private slots:
    void create_socket(int socket_fd);
    void delete_socket(editor::TcpSocket *socket);
    void serve_request(editor::TcpSocket *socket);

public slots:
    void dispatch_message(QSharedPointer<editor::Message> message);

public:
    Worker();
    void assign_connection(int socket_fd);
    unsigned int number_of_connections() const;
};
