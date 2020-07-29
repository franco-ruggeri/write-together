/*
 * Author: Franco Ruggeri
 */

#include "Worker.h"
#include "SafeSharedEditor.h"
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtGui/QImage>
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

// TODO: mancano risposte del protocollo (e.g. TextMessage dopo apertura)
// TODO: persistenza dei dati + modifiche a tutte le funzioni private
// TODO: nota su dummy editor (magari in descrizione classe?)
// TODO: metodo in SharedEditor per cursore (symbol -> index)
// TODO: c'e' codice duplicato in insert_symbol, erase_symbol, move_cursor e in login e signup
// TODO: aggiungi name, surname, etc. a UserProfile
// TODO: aggiungi site_id a protocollo
// TODO: modifica documentazione protocollo
// TODO: chiusura e distruzione socket (occhio al memory leakage)
// TODO: TextMessage e' sbagliato, devo inviare una sequenza di insert! il client deve avere i symbol
// TODO: logout => chiudi tutti i file di quell'utente
// TODO: verifica che il conteggio di riferimenti sia inizializzato a 0

// global data shared by workers
QSet<QString> online_users;
QHash<QString,QSharedPointer<SafeSharedEditor>> open_documents;
QHash<QString,unsigned int> reference_count;    // count references to open documents to know when to close
QMutex m_users, m_documents;

void Worker::assign_connection(qintptr socket_fd) {
    // the socket is not created directly here because it must belong to the thread the worker is running on
    emit new_connection(socket_fd);
}

void Worker::create_socket(int socket_fd) {
    QSharedPointer<TcpSocket> socket;
    try {
        socket = QSharedPointer<TcpSocket>::create(socket_fd);
    } catch (const std::exception& e) {
        qDebug() << e.what();
        return;     // this socket could not be created correctly, but the worker continues to serve other clients
    }

    connect(socket.get(), &TcpSocket::readyRead, this, &Worker::serve_request);
    connect(socket.get(), &TcpSocket::disconnected, socket.get(), &TcpSocket::deleteLater);

    QMutexLocker ml(&m_users_);
    users_.insert(socket_fd, User(socket));

    qDebug() << "connection accepted";
}

unsigned int Worker::number_of_connections() {
    QMutexLocker ml(&m_users_);
    return users_.size();
}

void Worker::dispatch_message(QSharedPointer<Message> message) {
    // get document
    QString document;
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
        case MessageType::close:
            document = message.staticCast<CloseMessage>()->document();
            break;
        default:
            throw std::logic_error("invalid message: invalid type to dispatch");
    }

    // dispatch to users editing the document
    QMutexLocker ml(&m_users_);
    for (auto it=users_.begin(); it != users_.end(); it++) {
        User& user = *it;
        QSharedPointer<TcpSocket> socket = user.socket();
        if (user.is_open(document))
            try {
                socket->write_message(message);
            } catch (const std::exception& e) {
                qDebug() << e.what();
                socket->disconnectFromHost();   // session compromised, the shared editor is not correct anymore
            }
    }
}

void Worker::serve_request() {
    // get user and socket
    int socket_fd = static_cast<TcpSocket*>(QObject::sender())->socketDescriptor();
    QMutexLocker ml(&m_users_);
    User& user = *users_.find(socket_fd);
    ml.unlock();
    QSharedPointer<TcpSocket> socket = user.socket();

    // not a whole message => wait next signal
    if (!socket->canReadLine()) return;

    // process message
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
}

void Worker::signup(User &user, QSharedPointer<Message> message) {
    // TODO: checks for valid usernames (e.g. not empty)
    // TODO: register profile on disk

    // check protocol errors
    if (user.authenticated()) std::logic_error("user already authenticated");

    // unpack message
    QSharedPointer<SignupMessage> m = message.staticCast<SignupMessage>();
    QString username = m->username();
    QString password = m->password();

    // register globally
    QMutexLocker ml(&m_users);
    if (online_users.contains(username)) throw std::runtime_error("user already existing");
    online_users.insert(username);
    ml.unlock();

    // register locally
    user.set_profile(UserProfile(username));
    user.set_authenticated(true);

    // send available documents
    QSharedPointer<TcpSocket> socket = user.socket();
    message = QSharedPointer<DocumentsMessage>::create(user.available_documents());
    socket->write_message(message);
}

void Worker::login(User &user, QSharedPointer<Message> message) {
    // TODO: check password and load profile

    // check protocol errors
    if (user.authenticated()) std::logic_error("user already authenticated");

    // unpack message
    QSharedPointer<LoginMessage> m = message.staticCast<LoginMessage>();
    QString username = m->username();
    QString password = m->password();

    // register globally
    QMutexLocker ml(&m_users);
    if (online_users.contains(username)) throw std::runtime_error("user already online");
    online_users.insert(username);
    ml.unlock();

    // register locally
    user.set_profile(UserProfile(username));
    user.set_authenticated(true);

    // send available documents
    QSharedPointer<TcpSocket> socket = user.socket();
    message = QSharedPointer<DocumentsMessage>::create(user.available_documents());
    socket->write_message(message);
}

void Worker::logout(User &user, QSharedPointer<Message> message) {
    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");

    // unregister globally
    QString username = user.profile().username();
    QMutexLocker ml(&m_users);
    online_users.remove(username);
    ml.unlock();

    // unregister locally
    user.clear();
}

void Worker::create_document(User &user, QSharedPointer<Message> message) {
    // TODO: modifica check, devo controllare che non ci sia sul disco, non in memoria

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");

    // unpack message
    QSharedPointer<CreateMessage> m = message.staticCast<CreateMessage>();
    QString document = m->document();

    // register globally
    QSharedPointer<SafeSharedEditor> local_editor = QSharedPointer<SafeSharedEditor>::create();
    QMutexLocker ml(&m_documents);
    if (open_documents.contains(document)) throw std::runtime_error("document already existing");
    open_documents.insert(document, local_editor);
    reference_count[document]++;
    ml.unlock();

    // register locally
    user.create_document(document, local_editor);
}

void Worker::open_document(User &user, QSharedPointer<Message> message) {
    // unpack message
    QSharedPointer<OpenMessage> m = message.staticCast<OpenMessage>();
    QString document = m->document();

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");
    if (!user.is_available(document)) throw std::logic_error("document not available");

    // register globally
    QMutexLocker ml(&m_documents);
    QSharedPointer<SafeSharedEditor> local_editor = open_documents[document];
    reference_count[document]++;
    ml.unlock();

    // register locally
    user.open_document(document, local_editor);
}

void Worker::insert_symbol(User &user, QSharedPointer<Message> message) {
    // unpack message
    QSharedPointer<InsertMessage> m = message.staticCast<InsertMessage>();
    QString document = m->document();
    Symbol symbol = m->symbol();

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");
    if (!user.is_open(document)) throw std::logic_error("document not opened");

    // insert locally
    QSharedPointer<SafeSharedEditor> local_editor = user.local_editor(document);
    local_editor->remote_insert(symbol);

    // notify insertion
    emit new_message(message);
}

void Worker::erase_symbol(User &user, QSharedPointer<Message> message) {
    // unpack message
    QSharedPointer<EraseMessage> m = message.staticCast<EraseMessage>();
    QString document = m->document();
    Symbol symbol = m->symbol();

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");
    if (!user.is_open(document)) throw std::logic_error("document not opened");

    // insert locally
    QSharedPointer<SafeSharedEditor> local_editor = user.local_editor(document);
    local_editor->remote_erase(symbol);

    // notify insertion
    emit new_message(message);
}

void Worker::move_cursor(User &user, QSharedPointer<Message> message) {
    // unpack message
    QSharedPointer<CursorMessage> m = message.staticCast<CursorMessage>();
    QString document = m->document();
    QString username = m->username();
    Symbol symbol = m->symbol();

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");
    if (!user.is_open(document)) throw std::logic_error("document not opened");

    // notify movement
    emit new_message(message);
}

void Worker::close_document(User &user, QSharedPointer<Message> message) {
    // unpack message
    QSharedPointer<CloseMessage> m = message.staticCast<CloseMessage>();
    QString document = m->document();
    QString username = m->username();

    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");
    if (!user.is_open(document)) throw std::logic_error("document not opened");

    // unregister globally
    QMutexLocker ml(&m_documents);
    if (--reference_count[document] <= 0) open_documents.remove(document);
    else emit new_message(message);
    ml.unlock();

    // unregister locally
    user.close_document(document);
}

void Worker::update_profile(User &user, QSharedPointer<Message> message) {
    // check protocol errors
    if (!user.authenticated()) throw std::logic_error("user not authenticated");

    // unpack message
    QSharedPointer<ProfileMessage> m = message.staticCast<ProfileMessage>();
    QString username = m->username();
    QString password = m->password();
    QImage icon = m->icon();

    // update in memory
    user.set_profile(UserProfile(username, icon));

    // TODO: update profile on disk
}
