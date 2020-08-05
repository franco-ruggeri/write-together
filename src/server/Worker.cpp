/*
 * Author: Franco Ruggeri
 */

#include "Worker.h"
#include "IdentityManager.h"
#include "DocumentManager.h"
#include <QtCore/QThread>
#include <QtNetwork/QHostAddress>
#include <cte/protocol/ErrorMessage.h>
#include <cte/protocol/SignupMessage.h>
#include <cte/protocol/SignupOkMessage.h>
#include <cte/protocol/LoginMessage.h>
#include <cte/protocol/LogoutMessage.h>
#include <cte/protocol/ProfileMessage.h>
#include <cte/protocol/ProfileOkMessage.h>
#include <cte/protocol/DocumentsMessage.h>
#include <cte/protocol/CreateMessage.h>
#include <cte/protocol/OpenMessage.h>
#include <cte/protocol/CloseMessage.h>
#include <cte/protocol/DocumentMessage.h>
#include <cte/protocol/InsertMessage.h>
#include <cte/protocol/EraseMessage.h>
#include <cte/protocol/CursorMessage.h>

// TODO: prova auto_increment, funziona?
// TODO: rifai foto ER diagram
// TODO: update github workflow to install boost (https://raymii.org/s/articles/Github_Actions_cpp_boost_cmake_speedup.html)
// TODO: alla fine confronta flusso con UML
// TODO: sharing_link -> QUrl
// TODO: DocumentData ha username->site_id, username->profile, si potrebbe fare username-><site_id,profile> con std::pair
// TODO: check site_id spoofing... in DocumentManager

IdentityManager identity_manager;
DocumentManager document_manager;

Worker::Worker() {
    connect(this, &Worker::new_connection, this, &Worker::start_session);
}

void Worker::assign_connection(int socket_fd) {
    /*
     * The socket is not created directly here because it must belong to the thread the worker is running on.
     * By means of the signal-slot mechanism, the socket will be created in create_socket() by the correct thread.
     */
    emit new_connection(socket_fd);
}

unsigned int Worker::number_of_connections() const {
    QMutexLocker ml(&m_number_of_connections_);
    return number_of_connections_;
}

void Worker::start_session(int socket_fd) {
    // create socket
    cte::TcpSocket *socket;
    try {
        socket = new cte::TcpSocket(socket_fd);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return; // this socket could not be created correctly, but the worker can continue to serve the other clients
    }

    // connect signals and slots (use socket_fd as session_id)
    connect(socket, &cte::TcpSocket::readyRead,
            [this, socket_fd, socket]() { serve_request(socket_fd, socket); });
    connect(socket, &cte::TcpSocket::disconnected,
            [this, socket_fd, socket]() { close_session(socket_fd, socket); });

    // increment number of sessions
    QMutexLocker ml(&m_number_of_connections_);
    number_of_connections_++;

    qDebug() << "new connection from: { address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort()
             << ", thread: " << QThread::currentThreadId() << "}";
}

void Worker::close_session(int session_id, cte::TcpSocket *socket) {
    // logout if authenticated
    if (identity_manager.authenticated(session_id))
        logout(session_id);

    // delete socket and session
    socket->deleteLater();

    // decrement number of sessions
    QMutexLocker ml(&m_number_of_connections_);
    number_of_connections_--;

    qDebug() << "connection closed: { address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort() << "}";
}

void Worker::dispatch_message(int source_socket_fd, QSharedPointer<cte::Message> message) {
    // get document
    cte::Document document;
    switch (message->type()) {
        case cte::MessageType::insert:
            document = message.staticCast<cte::InsertMessage>()->document();
            break;
        case cte::MessageType::erase:
            document = message.staticCast<cte::EraseMessage>()->document();
            break;
        case cte::MessageType::cursor:
            document = message.staticCast<cte::CursorMessage>()->document();
            break;
        case cte::MessageType::open:
            // TODO
            break;
        case cte::MessageType::close:
            document = message.staticCast<cte::CloseMessage>()->document();
            break;
        default:
            throw std::logic_error("invalid message: invalid type to dispatch");
    }

    // dispatch to clients editing the document
    auto it = editing_clients.find(document);
    if (it == editing_clients.end()) return;    // no editing clients
    for (cte::TcpSocket *socket : *it) {
        if (socket->socketDescriptor() == source_socket_fd) continue;
        try {
            socket->write_message(message);
        } catch (const std::exception &e) {
            // session compromised, the shared cte is not correct anymore
            qDebug() << "error -" << e.what();
            socket->disconnectFromHost();
        }
    }
}

static void send_error(cte::TcpSocket *socket, const QString& reason) {
    QSharedPointer<cte::Message> message = QSharedPointer<cte::ErrorMessage>::create(reason);
    socket->write_message(message);
}

void Worker::serve_request(int session_id, cte::TcpSocket *socket) {
    // not a whole message => wait next signal
    if (!socket->canReadLine()) return;

    // process message
    try {
        QSharedPointer<cte::Message> message = socket->read_message();
        switch (message->type()) {
            case cte::MessageType::signup:
                signup(session_id, socket, message);
                break;
            case cte::MessageType::login:
                login(session_id, socket, message);
                break;
            case cte::MessageType::logout:
                logout(session_id);
                break;
            case cte::MessageType::profile:
                update_profile(session_id, socket, message);
                break;
//            case MessageType::documents:
//                // TODO
//                break;
            case cte::MessageType::create:
                create_document(session_id, socket, message);
                break;
//            case MessageType::open:
//                open_document(user, message);
//                break;
//            case MessageType::close:
//                close_document(user, message);
//                break;
            case cte::MessageType::insert:
                insert_symbol(session_id, socket, message);
                break;
            case cte::MessageType::erase:
                erase_symbol(session_id, socket, message);
                break;
//            case MessageType::cursor:
//                move_cursor(user, message);
//                break;
            default:    // should never happen, since the message is generated through Message::deserialize
                throw std::logic_error("invalid message: invalid type");
        }
    } catch (const std::exception &e) {
        // log exception
        qDebug() << "error -" << e.what();

        // send error to client
        try {
            send_error(socket, e.what());
        } catch (const std::exception& e) {
            // just log, nothing to do, the client will not receive the error, but the server must continue (robust)
            qDebug() << e.what();
        }

        // close connection (network problem or bad client)
        socket->disconnectFromHost();
    }
}

void Worker::signup(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message) {
    // unpack message
    QSharedPointer<cte::SignupMessage> m = message.staticCast<cte::SignupMessage>();
    cte::Profile profile = m->profile();
    QString password = m->password();

    // signup
    if (!identity_manager.signup(session_id, profile, password)) {
        send_error(socket, "signup failed: username already used");
        return;
    }

    // send acknowledgement
    QSharedPointer<cte::Message> response = QSharedPointer<cte::SignupOkMessage>::create();
    socket->write_message(response);

    qDebug() << "signup by:" << profile.username();
}

void Worker::login(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message) {
    // unpack message
    QSharedPointer<cte::LoginMessage> m = message.staticCast<cte::LoginMessage>();
    QString username = m->username();
    QString password = m->password();

    // login
    std::optional<cte::Profile> profile = identity_manager.login(session_id, username, password);
    if (!profile) {
        send_error(socket, "login failed: wrong credentials");
        return;
    }

    // send profile
    QSharedPointer<cte::Message> response = QSharedPointer<cte::ProfileMessage>::create(*profile);
    socket->write_message(response);

    qDebug() << "login by:" << username;
}

void Worker::logout(int session_id) {
    // save username (just for final print)
    std::optional<QString> username = identity_manager.username(session_id);

    // logout
    identity_manager.logout(session_id);
    // TODO: close open documents

    qDebug() << "logout by:" << *username;
}

void Worker::update_profile(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message> &message) {
    // unpack message
    QSharedPointer<cte::ProfileMessage> m = message.staticCast<cte::ProfileMessage>();
    cte::Profile profile = m->profile();
    std::optional<QString> password = m->password();

    // save old username (just for final print)
    std::optional<QString> old_username = identity_manager.username(session_id);

    // update profile
    if (!identity_manager.update_profile(session_id, profile, password.value_or(QString{}))) {
        send_error(socket, "profile update failed: username already used");
        return;
    }

    // send acknowledgement
    QSharedPointer<cte::Message> response = QSharedPointer<cte::ProfileOkMessage>::create();
    socket->write_message(response);

    qDebug() << "profile update by:" << profile.username() << "(" + *old_username + ")";
}

void Worker::create_document(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message) {
    // get owner
    std::optional<QString> opt = identity_manager.username(session_id);
    if (!opt) throw std::logic_error("session not authenticated");
    QString document_owner = *opt;

    // unpack message
    QSharedPointer<cte::CreateMessage> m = message.staticCast<cte::CreateMessage>();
    QString document_name = m->document_name();

    // create document
    cte::Document document(document_owner, document_name);
    std::optional<cte::DocumentData> document_data = document_manager.create_document(session_id, document);
    if (!document_data) {
        send_error(socket, "document creation failed: document already existing");
        return;
    }

    // register for dispatching
    editing_clients.insert(document, QVector{socket});;

    // send acknowledgement
    QSharedPointer<cte::Message> response = QSharedPointer<cte::DocumentMessage>::create(document, *document_data);
    socket->write_message(response);

    qDebug() << "document created:" << document.full_name();
}

//void Worker::open_document(User &user, const QSharedPointer<Message>& message) {
//    TcpSocket *socket = user.socket();
//
//    // check protocol errors
//    if (!user.authenticated()) {
//        QString reason = "user not authenticated";
//        send_error(socket, reason);
//        throw std::logic_error(reason.toStdString());
//    }
//
//    // unpack message
//    QSharedPointer<OpenMessage> m = message.staticCast<OpenMessage>();
//    std::optional<Document>& document = m->document();
//    std::optional<QString>& sharing_link = m->sharing_link();
//
//    // check inputs
//    if (document && !user.is_accessible(document) || user.is_open(document)) {
//
//    }
//
//
//    QMutexLocker ml(&m_users);
//    if (online_users.contains(username)) {
//        QString reason = "user already online";
//        send_error(socket, reason);
//        return;
//    }
//
//    // TODO: load profile and check authentication
//    QSharedPointer<Profile> profile = QSharedPointer<Profile>::create(
//            username);    // TODO: must be changed with loaded profile
//
//    // register
//    online_users.insert(username, profile);
//    ml.unlock();
//    user.set_profile(profile);
//    user.set_authenticated(true);
//
//    // TODO: load accessible documents from disk
//    // user.set_accessible_documents(accessible_documents);     // TODO: uncomment
//
//    // send profile
//    QSharedPointer<Message> response = QSharedPointer<ProfileMessage>::create(*profile);
//    socket->write_message(response);
//
//    qDebug() << "login completed by:" << username;
//}
//
//void Worker::close_document(User& user, const QSharedPointer<Message>& message) {
////    // unpack message
////    QSharedPointer<CloseMessage> m = message.staticCast<CloseMessage>();
////    QString document = m->document();
////    QString username = m->username();
////
////    // check protocol errors
////    if (!user.authenticated()) throw std::logic_error("user not authenticated");
////    if (!user.is_open(document)) throw std::logic_error("document not opened");
////
////    // unregister globally
////    QMutexLocker ml(&m_documents);
////    if (--reference_count[document] <= 0) open_documents.remove(document);
////    else emit new_message(message);
////    ml.unlock();
////
////    // unregister locally
////    user.close_document(document);
//}
//
//void Worker::edit_document(const User &user, const Document& document, const Symbol& symbol,
//                           const QSharedPointer<Message>& message_to_dispatch,
//                           const std::function<void(const QSharedPointer<SafeSharedcte> &, const Symbol &)>& edit) {
//    TcpSocket *socket = user.socket();
//
//    // check protocol errors
//    if (!user.authenticated()) {
//        QString reason = "user already online";
//        send_error(socket, reason);
//        throw std::logic_error(reason.toStdString());
//    }
//    if (!user.is_open(document)) {
//        QString reason = "document not opened";
//        send_error(socket, reason);
//        throw std::logic_error(reason.toStdString());
//    }
//
//    // TODO: check site_id to avoid source spoofing
//
//    // edit document
//    QSharedPointer<SafeSharedcte> local_cte = user.cte(document);
//    edit(local_cte, symbol);
//
//    // notify insertion
//    emit new_message(message_to_dispatch);
//}

void Worker::insert_symbol(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message) {
    // check authentication
    if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

    // unpack message
    QSharedPointer<cte::InsertMessage> m = message.staticCast<cte::InsertMessage>();
    cte::Document document = m->document();
    cte::Symbol symbol = m->symbol();

    // insert symbol
    document_manager.insert_symbol(session_id, document, symbol);

    // dispatch message
    emit new_message(socket->socketDescriptor(), message);

    qDebug() << "insert: { document:" << document.full_name()
             << ", user:" << *identity_manager.username(session_id)
             << ", character:" << symbol.value() << "}";
}

void Worker::erase_symbol(int session_id, cte::TcpSocket *socket, const QSharedPointer<cte::Message>& message) {
    // check authentication
    if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

    // unpack message
    QSharedPointer<cte::EraseMessage> m = message.staticCast<cte::EraseMessage>();
    cte::Document document = m->document();
    cte::Symbol symbol = m->symbol();

    // insert symbol
    document_manager.erase_symbol(document, symbol);

    // dispatch message
    emit new_message(socket->socketDescriptor(), message);

    qDebug() << "erase: { document:" << document.full_name()
             << ", user:" << *identity_manager.username(session_id)
             << ", character:" << symbol.value() << "}";
}


//void Worker::move_cursor(const User& user, const QSharedPointer<Message>& message) {
//    QSharedPointer<CursorMessage> m = message.staticCast<CursorMessage>();
//    Document& document = m->document();
//    Symbol& symbol = m->symbol();
//    std::optional<QString>& u = m->username();
//
//    QString username = user.profile()->username();
//    if (u && *u != username)
//        throw std::logic_error("attempt of source username spoofing");
//    QSharedPointer<Message> message_to_dispatch = QSharedPointer<CursorMessage>::create(document, symbol, username);
//
//    edit_document(user, document, symbol, message_to_dispatch,
//                  // dummy edit, nothing to do in local copy
//                  [](const QSharedPointer<SafeSharedcte> &sse, const Symbol &s) {});
//
//    qDebug() << "cursor moved by:" << username;
//}
