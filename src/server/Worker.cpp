/*
 * Author: Franco Ruggeri
 */

#include "Worker.h"
#include "IdentityManager.h"
#include "DocumentManager.h"
#include <QtNetwork/QHostAddress>
#include <editor/protocol/ErrorMessage.h>
#include <editor/protocol/SignupMessage.h>
#include <editor/protocol/SignupOkMessage.h>
#include <editor/protocol/LoginMessage.h>
#include <editor/protocol/LogoutMessage.h>
#include <editor/protocol/ProfileMessage.h>
#include <editor/protocol/ProfileOkMessage.h>
#include <editor/protocol/DocumentsMessage.h>
#include <editor/protocol/CreateMessage.h>
#include <editor/protocol/OpenMessage.h>
#include <editor/protocol/CloseMessage.h>
#include <editor/protocol/DocumentMessage.h>
#include <editor/protocol/InsertMessage.h>
#include <editor/protocol/EraseMessage.h>
#include <editor/protocol/CursorMessage.h>

// TODO: prova auto_increment, funziona?
// TODO: update github workflow to install boost (https://raymii.org/s/articles/Github_Actions_cpp_boost_cmake_speedup.html)
// TODO: alla fine confronta flusso con UML
// TODO: sharing_link -> QUrl
// TODO: DocumentData ha username->site_id, username->profile, si potrebbe fare username-><site_id,profile> con std::pair

IdentityManager identity_manager;
DocumentManager document_manager;

Worker::Worker() : number_of_connections_(0) {
    connect(this, &Worker::new_connection, this, &Worker::create_socket);
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

void Worker::create_socket(int socket_fd) {
    // create socket
    editor::TcpSocket *socket;
    try {
        socket = new editor::TcpSocket(socket_fd);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return; // this socket could not be created correctly, but the worker can continue to serve the other clients
    }

    // connect signals and slots
    connect(socket, &editor::TcpSocket::readyRead, [this, socket]() { serve_request(socket); });
    connect(socket, &editor::TcpSocket::disconnected, [this, socket]() { delete_socket(socket); });

    // increment connection count
    QMutexLocker ml(&m_number_of_connections_);
    number_of_connections_++;

    qDebug() << "new connection from: { address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort() << "}";
}

void Worker::delete_socket(editor::TcpSocket *socket) {
    // logout if authenticated
    if (users_.contains(socket))
        logout(socket);

    // delete socket
    socket->deleteLater();

    // decrement connection count
    QMutexLocker ml(&m_number_of_connections_);
    number_of_connections_--;

    qDebug() << "connection closed: { address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort() << "}";
}

void Worker::dispatch_message(QSharedPointer<editor::Message> message) {
    // get document
    editor::Document document;
    switch (message->type()) {
        case editor::MessageType::insert:
            document = message.staticCast<editor::InsertMessage>()->document();
            break;
        case editor::MessageType::erase:
            document = message.staticCast<editor::EraseMessage>()->document();
            break;
        case editor::MessageType::cursor:
            document = message.staticCast<editor::CursorMessage>()->document();
            break;
        case editor::MessageType::open:
            // TODO
            break;
        case editor::MessageType::close:
            document = message.staticCast<editor::CloseMessage>()->document();
            break;
        default:
            throw std::logic_error("invalid message: invalid type to dispatch");
    }

    // dispatch to clients editing the document
//    auto it = editing_clients_.find(document);
//    if (it != editing_clients_.end()) {
//        QSet<TcpSocket *> &sockets = *it;
//        for (auto &socket : sockets)
//            try {
//                socket->write_message(message);
//            } catch (const std::exception &e) {
//                qDebug() << "error -" << e.what();
//                socket->disconnectFromHost();   // session compromised, the shared editor is not correct anymore
//            }
//    }
}

static void send_error(editor::TcpSocket *socket, const QString& reason) {
    QSharedPointer<editor::Message> message = QSharedPointer<editor::ErrorMessage>::create(reason);
    socket->write_message(message);
}

void Worker::serve_request(editor::TcpSocket *socket) {
    // not a whole message => wait next signal
    if (!socket->canReadLine()) return;

    // process message
    try {
        QSharedPointer<editor::Message> message = socket->read_message();
        switch (message->type()) {
            case editor::MessageType::signup:
                signup(socket, message);
                break;
            case editor::MessageType::login:
                login(socket, message);
                break;
            case editor::MessageType::logout:
                logout(socket);
                break;
            case editor::MessageType::profile:
                update_profile(socket, message);
                break;
//            case MessageType::documents:
//                // TODO
//                break;
            case editor::MessageType::create:
                create_document(socket, message);
                break;
//            case MessageType::open:
//                open_document(user, message);
//                break;
//            case MessageType::insert:
//                insert_symbol(user, message);
//                break;
//            case MessageType::erase:
//                erase_symbol(user, message);
//                break;
//            case MessageType::cursor:
//                move_cursor(user, message);
//                break;
//            case MessageType::close:
//                close_document(user, message);
//                break;
//            case MessageType::profile:
//                update_profile(user, message);
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
            qDebug() << e.what();
            // just log, nothing to do, the client will not receive the error, but the server must continue (robust)
        }

        // close connection (network problem or bad client)
        socket->disconnectFromHost();
    }
}

void Worker::signup(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message) {
    // check protocol errors
    if (users_.contains(socket))
        throw std::logic_error("protocol error: user already authenticated");

    // unpack message
    QSharedPointer<editor::SignupMessage> m = message.staticCast<editor::SignupMessage>();
    editor::Profile profile = m->profile();
    QString password = m->password();

    // signup
    if (!identity_manager.signup(profile, password)) {
        send_error(socket, "signup failed: username already used");
        return;
    }

    // bind socket to user
    users_.insert(socket, profile.username());

    // send acknowledgement
    QSharedPointer<editor::Message> response = QSharedPointer<editor::SignupOkMessage>::create();
    socket->write_message(response);

    qDebug() << "signup by:" << profile.username();
}

void Worker::login(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message) {
    // check protocol errors
    if (users_.contains(socket))
        throw std::logic_error("protocol error: user already authenticated");

    // unpack message
    QSharedPointer<editor::LoginMessage> m = message.staticCast<editor::LoginMessage>();
    QString username = m->username();
    QString password = m->password();

    // login
    std::optional<editor::Profile> profile = identity_manager.login(username, password);
    if (!profile) {
        send_error(socket, "login failed: wrong credentials");
        return;
    }

    // bind socket to user
    users_.insert(socket, username);

    // send profile
    QSharedPointer<editor::Message> response = QSharedPointer<editor::ProfileMessage>::create(*profile);
    socket->write_message(response);

    qDebug() << "login by:" << username;
}

void Worker::logout(editor::TcpSocket *socket) {
    // check protocol errors
    auto it = users_.find(socket);
    if (it == users_.end()) throw std::logic_error("protocol error: user not authenticated");
    QString username = it.value();  // by value for print at the end, don't use reference here

    // TODO: chiudi tutti i file aperti

    // unbind socket from user
    users_.remove(socket);

    qDebug() << "logout by:" << username;
}

void Worker::update_profile(editor::TcpSocket *socket, const QSharedPointer<editor::Message> &message) {
    // check protocol errors
    auto it = users_.find(socket);
    if (it == users_.end()) throw std::logic_error("protocol error: user not authenticated");
    QString& username = it.value();

    // unpack message
    QSharedPointer<editor::ProfileMessage> m = message.staticCast<editor::ProfileMessage>();
    editor::Profile profile = m->profile();
    std::optional<QString> password = m->password();

    // update profile
    if (!identity_manager.update_profile(username, profile, password.value_or(QString{}))) {
        send_error(socket, "profile update failed: username already used");
        return;
    }

    // send acknowledgement
    QSharedPointer<editor::Message> response = QSharedPointer<editor::ProfileOkMessage>::create();
    socket->write_message(response);

    qDebug() << "profile update by:" << profile.username() << "(" + username + ")";
}

void Worker::create_document(editor::TcpSocket *socket, const QSharedPointer<editor::Message>& message) {
    // check protocol errors
    auto it = users_.find(socket);
    if (it == users_.end()) throw std::logic_error("protocol error: user not authenticated");
    QString& owner = it.value();

    // unpack message
    QSharedPointer<editor::CreateMessage> m = message.staticCast<editor::CreateMessage>();
    QString name = m->document_name();
    editor::Document document(owner, name);

    // create document
    std::optional<editor::DocumentData> document_data = document_manager.create_document(document);
    if (!document_data) {
        send_error(socket, "document creation failed: document already existing");
        return;
    }

    // send acknowledgement
    QSharedPointer<editor::Message> response = QSharedPointer<editor::DocumentMessage>::create(document, *document_data);
    socket->write_message(response);

    qDebug() << "document created:" << document.full_name();
}

//void Worker::create_document(User &user, const QSharedPointer<Message>& message) {
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
//    QSharedPointer<CreateMessage> m = message.staticCast<CreateMessage>();
//    QString& name = m->document_name();
//    QString owner = user.profile()->username();
//
//    // check inputs
//    if (name.isEmpty()) {
//        send_error(socket, "invalid document name");
//        return;
//    }
//
//    // TODO: check if document already exists on disk
//
//    // register
//    Document document(owner, name);
//    QMutexLocker ml(&m_documents);
//    open_documents.insert(document, OpenDocument{});
//    ml.unlock();
//    // TODO: register document on disk
//
//    // send document
//    // TODO
////    QSharedPointer<Message> response = QSharedPointer<DocumentMessage>::create();
////    socket->write_message(response);
//
//    qDebug() << "document created:" << document.full_name();
//}
//
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
//                           const std::function<void(const QSharedPointer<SafeSharedEditor> &, const Symbol &)>& edit) {
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
//    QSharedPointer<SafeSharedEditor> local_editor = user.editor(document);
//    edit(local_editor, symbol);
//
//    // notify insertion
//    emit new_message(message_to_dispatch);
//}
//
//
//void Worker::insert_symbol(const User& user, const QSharedPointer<Message>& message) {
//    QSharedPointer<InsertMessage> m = message.staticCast<InsertMessage>();
//    Document& document = m->document();
//    Symbol& symbol = m->symbol();
//
//    edit_document(user, document, symbol, message, [](const QSharedPointer<SafeSharedEditor>& sse, const Symbol& s) {
//        sse->remote_insert(s);
//    });
//
//    qDebug() << "symbol inserted by:" << user.profile()->username();
//}
//
//void Worker::erase_symbol(const User& user, const QSharedPointer<Message>& message) {
//    QSharedPointer<EraseMessage> m = message.staticCast<EraseMessage>();
//    Document& document = m->document();
//    Symbol& symbol = m->symbol();
//
//    edit_document(user, document, symbol, message, [](const QSharedPointer<SafeSharedEditor>& sse, const Symbol& s) {
//        sse->remote_erase(s);
//    });
//
//    qDebug() << "symbol erased by:" << user.profile()->username();
//}
//
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
//                  [](const QSharedPointer<SafeSharedEditor> &sse, const Symbol &s) {});
//
//    qDebug() << "cursor moved by:" << username;
//}
