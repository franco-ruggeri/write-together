/*
 * Author: Franco Ruggeri
 */

#include <QtCore/QThread>
#include <QtNetwork/QHostAddress>
#include <cte/server/Worker.h>
#include <cte/server/IdentityManager.h>
#include <cte/server/DocumentManager.h>
#include <cte/protocol/ErrorMessage.h>
#include <cte/protocol/SignupMessage.h>
#include <cte/protocol/SignupOkMessage.h>
#include <cte/protocol/LoginMessage.h>
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

namespace cte {
    extern IdentityManager identity_manager;
    extern DocumentManager document_manager;

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
        TcpSocket *socket;
        try {
            socket = new TcpSocket(socket_fd);
        } catch (const std::exception &e) {
            qDebug() << e.what();
            return; // this socket could not be created correctly, but the worker can continue to serve the other clients
        }

        // connect signals and slots (use socket_fd as session_id)
        connect(socket, &TcpSocket::readyRead,
                [this, socket_fd, socket]() { serve_request(socket_fd, socket); });
        connect(socket, &TcpSocket::disconnected,
                [this, socket_fd, socket]() { close_session(socket_fd, socket); });

        // increment number of sessions
        QMutexLocker ml(&m_number_of_connections_);
        number_of_connections_++;

        qDebug() << "new connection from: { address:" << socket->peerAddress().toString()
                 << ", port:" << socket->peerPort()
                 << ", thread: " << QThread::currentThreadId() << "}";
    }

    void Worker::close_session(int session_id, TcpSocket *socket) {
        // logout
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

    void Worker::dispatch_message(int source_socket_fd, QSharedPointer<Message> message) {
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
                document = message.staticCast<CursorMessage>()->document();
                break;
            case MessageType::close:
                document = message.staticCast<CloseMessage>()->document();
                break;
            default:
                throw std::logic_error("invalid message: invalid type to dispatch");
        }

        // dispatch to clients editing the document
        auto it = editing_clients.find(document);
        if (it == editing_clients.end()) return;    // no editing clients
        for (TcpSocket *socket : *it) {
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

    static void send_error(TcpSocket *socket, const QString& reason) {
        QSharedPointer<Message> message = QSharedPointer<ErrorMessage>::create(reason);
        socket->write_message(message);
    }

    void Worker::serve_request(int session_id, TcpSocket *socket) {
        // not a whole message => wait next signal
        if (!socket->canReadLine()) return;

        // process message
        try {
            QSharedPointer<Message> message = socket->read_message();
            switch (message->type()) {
                case MessageType::signup:
                    signup(session_id, socket, message);
                    break;
                case MessageType::login:
                    login(session_id, socket, message);
                    break;
                case MessageType::logout:
                    logout(session_id);
                    break;
                case MessageType::profile:
                    update_profile(session_id, socket, message);
                    break;
                case MessageType::documents:
                    accessible_documents(session_id, socket, message);
                    break;
                case MessageType::create:
                    create_document(session_id, socket, message);
                    break;
                case MessageType::open:
                    open_document(session_id, socket, message);
                    break;
                case MessageType::close:
                    close_document(session_id, socket, message);
                    break;
                case MessageType::insert:
                    insert_symbol(session_id, socket, message);
                    break;
                case MessageType::erase:
                    erase_symbol(session_id, socket, message);
                    break;
                case MessageType::cursor:
                    move_cursor(session_id, socket, message);
                    break;
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

    void Worker::signup(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // unpack message
        QSharedPointer<SignupMessage> signup_message = message.staticCast<SignupMessage>();
        Profile profile = signup_message->profile();
        QString password = signup_message->password();

        // signup
        if (!identity_manager.signup(session_id, profile, password)) {
            send_error(socket, "signup failed: username already used");
            return;
        }

        // send acknowledgement
        QSharedPointer<Message> response = QSharedPointer<SignupOkMessage>::create();
        socket->write_message(response);

        qDebug() << "signup by:" << profile.username();
    }

    void Worker::login(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // unpack message
        QSharedPointer<LoginMessage> login_message = message.staticCast<LoginMessage>();
        QString username = login_message->username();
        QString password = login_message->password();

        // login
        std::optional<Profile> profile = identity_manager.login(session_id, username, password);
        if (!profile) {
            send_error(socket, "login failed: wrong credentials");
            return;
        }

        // send profile
        QSharedPointer<Message> response = QSharedPointer<ProfileMessage>::create(*profile);
        socket->write_message(response);

        qDebug() << "login by:" << username;
    }

    void Worker::logout(int session_id) {
        // save username (just for final print)
        std::optional<QString> username = identity_manager.username(session_id);

        // close open documents and logout
        document_manager.close_documents(session_id);
        identity_manager.logout(session_id);

        qDebug() << "logout by:" << *username;
    }

    void Worker::update_profile(int session_id, TcpSocket *socket, const QSharedPointer<Message> &message) {
        // unpack message
        QSharedPointer<ProfileMessage> profile_message = message.staticCast<ProfileMessage>();
        Profile profile = profile_message->profile();
        std::optional<QString> password = profile_message->password();

        // save old username (just for final print)
        std::optional<QString> old_username = identity_manager.username(session_id);

        // update profile
        if (!identity_manager.update_profile(session_id, profile, password.value_or(QString{}))) {
            send_error(socket, "profile update failed: username already used");
            return;
        }

        // send acknowledgement
        QSharedPointer<Message> response = QSharedPointer<ProfileOkMessage>::create();
        socket->write_message(response);

        qDebug() << "profile update by:" << profile.username() << "(" + *old_username + ")";
    }

    void Worker::create_document(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // get user
        std::optional<QString> opt = identity_manager.username(session_id);
        if (!opt) throw std::logic_error("session not authenticated");
        QString document_owner = *opt;

        // unpack message
        QSharedPointer<CreateMessage> create_message = message.staticCast<CreateMessage>();
        QString document_name = create_message->document_name();

        // create document
        Document document(document_owner, document_name);
        std::optional<DocumentData> document_data = document_manager.create_document(session_id, document);
        if (!document_data) {
            send_error(socket, "document creation failed: document already existing");
            return;
        }

        // register for dispatching
        editing_clients.insert(document, QSet{socket});;

        // send document data
        QSharedPointer<Message> response = QSharedPointer<DocumentMessage>::create(document, *document_data);
        socket->write_message(response);

        qDebug() << "document created:" << document.full_name();
    }

    void Worker::open_document(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // get user
        std::optional<QString> opt = identity_manager.username(session_id);
        if (!opt) throw std::logic_error("session not authenticated");
        QString username = *opt;

        // unpack message
        QSharedPointer<OpenMessage> open_message = message.staticCast<OpenMessage>();
        std::optional<Document> document = open_message->document();
        if (!document) {
            std::optional<QUrl> sharing_link = open_message->sharing_link();
            if (!sharing_link) throw std::logic_error("invalid message: open with neither document nor sharing link");
            document = document_manager.document(*sharing_link);
        }

        // open document
        std::optional<DocumentData> document_data;
        document_data = document_manager.open_document(session_id, *document, username);
        if (!document_data) {
            send_error(socket, "document open failed: document not existing or not accessible");
            return;
        }

        // register for dispatching
        editing_clients[*document].insert(socket);

        // send document data
        QSharedPointer<Message> response = QSharedPointer<DocumentMessage>::create(*document, *document_data);
        socket->write_message(response);

        // dispatch message
        open_message->set_site_id(document_data->site_id());
        open_message->set_profile(*document_data->profiles().find(username));
        emit new_message(socket->socketDescriptor(), open_message);

        qDebug() << "document opened: { document:" << document->full_name() << ", user:" << username << "}";
    }

    void Worker::close_document(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // check authentication
        if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

        // unpack message
        QSharedPointer<CloseMessage> close_message = message.staticCast<CloseMessage>();
        Document document = close_message->document();

        // close document
        document_manager.close_document(session_id, document);

        // unregister for dispatching
        editing_clients[document].remove(socket);

        // dispatch message
        QString username = *identity_manager.username(session_id);
        close_message->set_username(username);
        emit new_message(socket->socketDescriptor(), close_message);

        qDebug() << "document closed: { document:" << document.full_name() << ", user:" << username << "}";
    }

    void Worker::accessible_documents(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // get user
        std::optional<QString> opt = identity_manager.username(session_id);
        if (!opt) throw std::logic_error("session not authenticated");
        QString username = *opt;

        // get documents
        QSet<Document> documents = document_manager.documents(session_id, username);

        // send documents
        QSharedPointer<Message> response = QSharedPointer<DocumentsMessage>::create(documents);
        socket->write_message(response);
    }

    void Worker::insert_symbol(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // check authentication
        if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

        // unpack message
        QSharedPointer<InsertMessage> insert_message = message.staticCast<InsertMessage>();
        Document document = insert_message->document();
        Symbol symbol = insert_message->symbol();

        // insert symbol
        document_manager.insert_symbol(session_id, document, symbol);

        // dispatch message
        emit new_message(socket->socketDescriptor(), insert_message);

        qDebug() << "insert: { document:" << document.full_name()
                 << ", user:" << *identity_manager.username(session_id)
                 << ", character:" << symbol.value() << "}";
    }

    void Worker::erase_symbol(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // check authentication
        if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

        // unpack message
        QSharedPointer<EraseMessage> erase_message = message.staticCast<EraseMessage>();
        Document document = erase_message->document();
        Symbol symbol = erase_message->symbol();

        // erase symbol
        document_manager.erase_symbol(session_id, document, symbol);

        // dispatch message
        emit new_message(socket->socketDescriptor(), erase_message);

        qDebug() << "erase: { document:" << document.full_name()
                 << ", user:" << *identity_manager.username(session_id)
                 << ", character:" << symbol.value() << "}";
    }

    void Worker::move_cursor(int session_id, TcpSocket *socket, const QSharedPointer<Message>& message) {
        // check authentication
        if (!identity_manager.authenticated(session_id)) throw std::logic_error("session not authenticated");

        // unpack message
        QSharedPointer<CursorMessage> cursor_message = message.staticCast<CursorMessage>();
        Document document = cursor_message->document();
        Symbol symbol = cursor_message->symbol();

        // move cursor
        document_manager.move_cursor(session_id, document, symbol);

        // dispatch message
        QString username = *identity_manager.username(session_id);
        cursor_message->set_username(username);
        emit new_message(socket->socketDescriptor(), cursor_message);

        qDebug() << "move cursor: { document:" << document.full_name()
                 << ", user:" << *identity_manager.username(session_id)
                 << ", character:" << symbol.value() << "}";
    }
}
