/*
 * Author: Franco Ruggeri
 */

#include "Worker.h"
#include <QtNetwork/QHostAddress>
#include <protocol/ErrorMessage.h>
#include <protocol/SignupMessage.h>
#include <protocol/LoginMessage.h>
#include <protocol/LogoutMessage.h>
#include <protocol/CreateMessage.h>
#include <protocol/OpenMessage.h>
#include <protocol/InsertMessage.h>
#include <protocol/EraseMessage.h>
#include <protocol/CursorMessage.h>
#include <protocol/CloseMessage.h>
#include <protocol/ProfileMessage.h>
#include <protocol/DocumentsMessage.h>

// TODO: logout e delete_socket => chiudi tutti i file di quell'utente

// TODO: mancano risposte del protocollo (e.g. TextMessage dopo apertura)
// TODO: persistenza dei dati + modifiche a tutte le funzioni private, classe Saver + thread dedicato (che usa le variabili globali con extern)
// TODO: update github workflow to install boost (https://raymii.org/s/articles/Github_Actions_cpp_boost_cmake_speedup.html)
// TODO: nota su dummy editor (magari in descrizione classe?)
// TODO: codice duplicato in login e signup, quando sono complete valuta se fare una funzione di supporto come in insert e erase

// TODO: alla fine confronta flusso con UML

QHash<QString, QSharedPointer<Profile>> online_users;
QHash<Document, OpenDocument> open_documents;
QMutex m_users, m_documents;

Worker::Worker() {
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
    return users_.size();
}

void Worker::create_socket(int socket_fd) {
    // create socket
    TcpSocket *socket;
    try {
        socket = new TcpSocket(socket_fd);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return; // this socket could not be created correctly, but the worker can continue to serve the other clients
    }

    // connect signals and slots
    connect(socket, &TcpSocket::readyRead, [this, socket]() {
        QMutexLocker ml(&m_users_);
        auto it = users_.find(socket);
        ml.unlock();
        serve_request(*it);
    });
    connect(socket, &TcpSocket::disconnected, [this, socket]() { delete_socket(socket); });

    // create user
    QMutexLocker ml(&m_users_);
    users_.insert(socket, User(socket));

    qDebug() << "new connection from: { ip address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort() << "}";
}

void Worker::delete_socket(TcpSocket *socket) {
    // close documents
    // TODO

    // delete socket
    socket->deleteLater();
    for (auto &sockets : editing_clients_) {
        auto it = sockets.find(socket);
        if (it != sockets.end()) sockets.erase(it);
    }
    QMutexLocker ml(&m_users_);
    users_.remove(socket);

    qDebug() << "connection closed: { ip address:" << socket->peerAddress().toString()
             << ", port:" << socket->peerPort() << "}";
}

void Worker::dispatch_message(QSharedPointer<Message> message) {
    // get document
    Document document;
    switch (message->type()) {
        case MessageType::insert:
            document = message.staticCast<InsertMessage>()->document();
            break;
        case MessageType::erase:
            document = message.staticCast<EraseMessage>()->document();
            break;
        case MessageType::cursor:
            document = message.staticCast<CursorMessage>()->document();
            break;
        case MessageType::open:
            // TODO
            break;
        case MessageType::close:
            document = message.staticCast<CloseMessage>()->document();
            break;
        default:
            throw std::logic_error("invalid message: invalid type to dispatch");
    }

    // dispatch to clients editing the document
    auto it = editing_clients_.find(document);
    if (it != editing_clients_.end()) {
        QSet<TcpSocket *> &sockets = *it;
        for (auto &socket : sockets)
            try {
                socket->write_message(message);
            } catch (const std::exception &e) {
                qDebug() << "error:" << e.what();
                socket->disconnectFromHost();   // session compromised, the shared editor is not correct anymore
            }
    }
}

void Worker::serve_request(User &user) {
    TcpSocket *socket = user.socket();

    // not a whole message => wait next signal
    if (!socket->canReadLine()) return;

    // process message
    try {
        QSharedPointer<Message> message = socket->read_message();
        switch (message->type()) {
            case MessageType::signup:
                signup(user, message);
                break;
            case MessageType::login:
                login(user, message);
                break;
            case MessageType::logout:
                logout(user, message);
                break;
            case MessageType::documents:
                // TODO
                break;
            case MessageType::create:
                create_document(user, message);
                break;
            case MessageType::open:
                open_document(user, message);
                break;
            case MessageType::insert:
                insert_symbol(user, message);
                break;
            case MessageType::erase:
                erase_symbol(user, message);
                break;
            case MessageType::cursor:
                move_cursor(user, message);
                break;
            case MessageType::close:
                close_document(user, message);
                break;
            case MessageType::profile:
                update_profile(user, message);
                break;
            default:    // should never happen, since the message is generated through Message::deserialize
                throw std::logic_error("invalid message: invalid type");
        }
    } catch (const std::exception &e) {
        qDebug() << "error:" << e.what();
        socket->disconnectFromHost();
    }
}

static void send_error(TcpSocket *socket, const QString& reason) {
    QSharedPointer<Message> message = QSharedPointer<ErrorMessage>::create(reason);
    socket->write_message(message);
}

void Worker::signup(User &user, const QSharedPointer<Message>& message) {
    TcpSocket *socket = user.socket();

    // check protocol errors
    if (user.authenticated()) {
        QString reason = "user already authenticated";
        send_error(socket, reason);
        throw std::logic_error(reason.toStdString());
    }

    // unpack message
    QSharedPointer<SignupMessage> m = message.staticCast<SignupMessage>();
    QString& username = m->username();
    QString& password = m->password();

    // check inputs
    if (username.isEmpty() || password.isEmpty()) {
        send_error(socket, "invalid username or password");
        return;
    }

    // TODO: check if username already exists on disk

    // register
    QSharedPointer<Profile> profile = QSharedPointer<Profile>::create(username);
    QMutexLocker ml(&m_users);
    online_users.insert(username, profile);
    ml.unlock();
    user.set_profile(profile);
    user.set_authenticated(true);
    // TODO: register profile on disk

    // send profile
    QSharedPointer<Message> response = QSharedPointer<ProfileMessage>::create(*profile);
    socket->write_message(response);

    qDebug() << "signup completed by:" << username;
}

void Worker::login(User &user, const QSharedPointer<Message>& message) {
    TcpSocket *socket = user.socket();

    // check protocol errors
    if (user.authenticated()) {
        QString reason = "user already authenticated";
        send_error(socket, reason);
        throw std::logic_error(reason.toStdString());
    }

    // unpack message
    QSharedPointer<LoginMessage> m = message.staticCast<LoginMessage>();
    QString& username = m->username();
    QString& password = m->password();

    // check inputs
    QMutexLocker ml(&m_users);
    if (online_users.contains(username)) {
        QString reason = "user already online";
        send_error(socket, reason);
        return;
    }

    // TODO: load profile and check authentication
    QSharedPointer<Profile> profile = QSharedPointer<Profile>::create(
            username);    // TODO: must be changed with loaded profile

    // register
    online_users.insert(username, profile);
    ml.unlock();
    user.set_profile(profile);
    user.set_authenticated(true);

    // TODO: load accessible documents
    // user.set_accessible_documents(accessible_documents);     // TODO: uncomment

    // send profile
    QSharedPointer<Message> response = QSharedPointer<ProfileMessage>::create(*profile);
    socket->write_message(response);

    qDebug() << "login completed by:" << username;
}

void Worker::logout(User &user, const QSharedPointer<Message>& message) {
//    // check protocol errors
//    if (!user.authenticated()) throw std::logic_error("user not authenticated");
//
//    // unregister globally
//    QString username = user.profile().username();
//    QMutexLocker ml(&m_users);
//    online_users.remove(username);
//    ml.unlock();
//
//    // unregister locally
//    user.clear();
}

void Worker::update_profile(User& user, const QSharedPointer<Message>& message) {
//    // check protocol errors
//    if (!user.authenticated()) throw std::logic_error("user not authenticated");
//
//    // unpack message
//    QSharedPointer<ProfileMessage> m = message.staticCast<ProfileMessage>();
//    QString username = m->username();
//    QString password = m->password();
//    QImage icon = m->icon();
//
//    // update in memory
//    user.set_profile(Profile(username, icon));
//
//    // TODO: update profile on disk
}

void Worker::create_document(User &user, const QSharedPointer<Message>& message) {

//    // TODO: modifica check, devo controllare che non ci sia sul disco, non in memoria
//
//    // check protocol errors
//    if (!user.authenticated()) throw std::logic_error("user not authenticated");
//
//    // unpack message
//    QSharedPointer<CreateMessage> m = message.staticCast<CreateMessage>();
//    QString document = m->document_name();
//
//    // register globally
//    QSharedPointer<SafeSharedEditor> local_editor = QSharedPointer<SafeSharedEditor>::create();
//    QMutexLocker ml(&m_documents);
//    if (open_documents.contains(document)) throw std::runtime_error("document already existing");
//    open_documents.insert(document, local_editor);
//    reference_count[document]++;
//    ml.unlock();
//
//    // register locally
//    user.create_document(document, local_editor);
}

void Worker::open_document(User &user, const QSharedPointer<Message>& message) {
//    // unpack message
//    QSharedPointer<OpenMessage> m = message.staticCast<OpenMessage>();
//    QString document = m->document();
//
//    // check protocol errors
//    if (!user.authenticated()) throw std::logic_error("user not authenticated");
//    if (!user.is_available(document)) throw std::logic_error("document not available");
//
//    // register globally
//    QMutexLocker ml(&m_documents);
//    QSharedPointer<SafeSharedEditor> local_editor = open_documents[document];
//    reference_count[document]++;
//    ml.unlock();
//
//    // register locally
//    user.open_document(document, local_editor);
}

void Worker::close_document(User& user, const QSharedPointer<Message>& message) {
//    // unpack message
//    QSharedPointer<CloseMessage> m = message.staticCast<CloseMessage>();
//    QString document = m->document();
//    QString username = m->username();
//
//    // check protocol errors
//    if (!user.authenticated()) throw std::logic_error("user not authenticated");
//    if (!user.is_open(document)) throw std::logic_error("document not opened");
//
//    // unregister globally
//    QMutexLocker ml(&m_documents);
//    if (--reference_count[document] <= 0) open_documents.remove(document);
//    else emit new_message(message);
//    ml.unlock();
//
//    // unregister locally
//    user.close_document(document);
}

void Worker::edit_document(const User &user, const Document& document, const Symbol& symbol,
                           const QSharedPointer<Message>& message_to_dispatch,
                           const std::function<void(const QSharedPointer<SafeSharedEditor> &, const Symbol &)>& edit) {
    TcpSocket *socket = user.socket();

    // check protocol errors
    if (!user.authenticated()) {
        QString reason = "user already online";
        send_error(socket, reason);
        throw std::logic_error(reason.toStdString());
    }
    if (!user.is_open(document)) {
        QString reason = "document not opened";
        send_error(socket, reason);
        throw std::logic_error(reason.toStdString());
    }

    // TODO: check site_id to avoid source spoofing

    // edit document
    QSharedPointer<SafeSharedEditor> local_editor = user.editor(document);
    edit(local_editor, symbol);

    // notify insertion
    emit new_message(message_to_dispatch);
}


void Worker::insert_symbol(const User& user, const QSharedPointer<Message>& message) {
    QSharedPointer<InsertMessage> m = message.staticCast<InsertMessage>();
    Document& document = m->document();
    Symbol& symbol = m->symbol();

    edit_document(user, document, symbol, message, [](const QSharedPointer<SafeSharedEditor>& sse, const Symbol& s) {
        sse->remote_insert(s);
    });
}

void Worker::erase_symbol(const User& user, const QSharedPointer<Message>& message) {
    QSharedPointer<EraseMessage> m = message.staticCast<EraseMessage>();
    Document& document = m->document();
    Symbol& symbol = m->symbol();

    edit_document(user, document, symbol, message, [](const QSharedPointer<SafeSharedEditor>& sse, const Symbol& s) {
        sse->remote_erase(s);
    });
}

void Worker::move_cursor(const User& user, const QSharedPointer<Message>& message) {
    QSharedPointer<CursorMessage> m = message.staticCast<CursorMessage>();
    Document& document = m->document();
    Symbol& symbol = m->symbol();
    std::optional<QString>& username = m->username();

    QString correct_username = user.profile()->username();
    if (username && *username != correct_username)
        throw std::logic_error("attempt of source username spoofing");
    QSharedPointer<Message> message_to_dispatch = QSharedPointer<CursorMessage>::create(document, symbol,
                                                                                        correct_username);

    edit_document(user, document, symbol, message_to_dispatch,
                  // dummy edit, nothing to do in local copy
                  [](const QSharedPointer<SafeSharedEditor> &sse, const Symbol &s) {});
}
