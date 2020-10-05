/*
 * Author: Antonino Musmeci, Stefano Di Blasio
 */
//TODO: add user name
//TODO add error message
//TODO add cursor message
//TODO add insert icon
#include <cte/client/myClient.h>
#include <QPixmap>
#include <QJsonDocument>
#include <QtCore/QSettings>
#include <cte/protocol/MessageType.h>
#include <cte/protocol/OpenMessage.h>
#include <cte/protocol/ProfileMessage.h>
#include <cte/client/fileInfo.h>
#include <cte/protocol/CloseMessage.h>
#include <cte/protocol/CursorMessage.h>
#include <cte/protocol/DocumentMessage.h>
#include <cte/protocol/SignupMessage.h>
#include <cte/protocol/LogoutMessage.h>
#include <cte/protocol/InsertMessage.h>
#include <cte/protocol/CreateMessage.h>
#include <cte/protocol/EraseMessage.h>
#include <cte/protocol/LoginMessage.h>
#include <cte/protocol/ErrorMessage.h>
#include <cte/protocol/DocumentsMessage.h>
#include <iostream>


using namespace cte;
myClient::myClient(QObject *parent) : QObject(parent) {
    try {
        socket = new Socket();
        wait_on_connection_ = QSharedPointer<QTimer>::create();
        wait_on_connection_->setInterval(10000);
        wait_on_connection_->setSingleShot(true);
        connecting_interrupt_ = QSharedPointer<QTimer>::create();
        connecting_interrupt_->setSingleShot(true);
        QSettings settings;
        host_address_ = settings.value("client/hostname").toString();
        fallback_host_address_ = settings.value("client/secondhostname", "localhost").toString();
        port_ = settings.value("client/port").toUInt();
        ssl_handshake_failed_ = false;
        QObject::connect(socket, &Socket::ready_message, wait_on_connection_.get(), &QTimer::stop);
        QObject::connect(socket, &Socket::ready_message, this, &myClient::process_response);
        QObject::connect(wait_on_connection_.get(), &QTimer::timeout, this, &myClient::attempt_timeout);
        // establish connection and handshake
//        QObject::connect(socket, &QAbstractSocket::errorOccurred, this, &myClient::handle_connection_error); // supported as of qt 5.15
        QObject::connect(socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), this, &myClient::handle_ssl_handshake);
        QObject::connect(socket, &QSslSocket::encrypted, this, &myClient::connection_encrypted);
        // QObject::connect(socket, &QAbstractSocket::connected, this, [this](){ qDebug() << "Connection achieved to host " << this->host_to_connect_;}); // this is the right one
//        QObject::connect(socket, &QAbstractSocket::connected, this, &myClient::connection_encrypted); // this is only because encryption does not work at the moment
        // the latter is temporally substituted by qt backward compatible slot connected to state changes of socket
        // the followings are for qt backward compatibility
        QObject::connect(socket, &QAbstractSocket::stateChanged, this, &myClient::handle_changed_state);
        QObject::connect(connecting_interrupt_.get(), &QTimer::timeout, this, &myClient::timeout_on_connection);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return;
    }
}

myClient::~myClient() noexcept {
    if (socket) socket->deleteLater();
    this->deleteLater();
}

void myClient::handle_connection_error(QAbstractSocket::SocketError error) {
    qDebug() << "Verified connection error of type " << static_cast<int>(error) << " for host to connect to " << host_to_connect_;
    if (error == QAbstractSocket::HostNotFoundError && host_to_connect_ == host_address_) {
        host_to_connect_ = fallback_host_address_;
        this->connect();
        return;
    } else if (error == QAbstractSocket::SslHandshakeFailedError) { // this should not happen if server is configured
        ssl_handshake_failed_ = true;
    } else if (error == QAbstractSocket::RemoteHostClosedError && ssl_handshake_failed_) { // fallback in case of previous condition
        qWarning() << "Unable to connect to the server because of SSL handshake failed";
    } else {
        ssl_handshake_failed_ = false;
        host_to_connect_ = "";
    }
    emit host_connected(false);
}

void myClient::handle_changed_state(QAbstractSocket::SocketState new_state) {
    if (new_state == QAbstractSocket::HostLookupState) {
        previous_state_ = QAbstractSocket::HostLookupState;
        connecting_interrupt_->start(5000);
    } else if (previous_state_ == QAbstractSocket::HostLookupState && new_state == QAbstractSocket::ConnectingState) {
        previous_state_ = QAbstractSocket::ConnectingState;
        connecting_interrupt_->start(10000);
    } else if (previous_state_ == QAbstractSocket::ConnectingState && new_state == QAbstractSocket::ConnectedState) {
        connecting_interrupt_->stop();
        qDebug() << "The socket is connected to the server";
//        qDebug() << "Starting SSL handshake";
        emit host_connected(true); // only for connect without encryption
    } else if (new_state == QAbstractSocket::ClosingState) {
        qDebug() << "About to close the connection";
        previous_state_ = QAbstractSocket::ClosingState;
    } else if (new_state == QAbstractSocket::UnconnectedState) {
        if (previous_state_ == QAbstractSocket::HostLookupState && host_to_connect_ == host_address_) { // primary address DNS not resolved
            previous_state_ = QAbstractSocket::UnconnectedState;
            host_to_connect_ = fallback_host_address_;
            connecting_interrupt_->start(5000);
            this->connect();
        } else if (previous_state_ == QAbstractSocket::ClosingState) {
            qDebug() << "Successfully closed the connection with host " << host_to_connect_ << ":" << port_;
            emit server_disconnected();
        } else { // the connection wasn't established
            previous_state_ = QAbstractSocket::UnconnectedState;
            qDebug() << "Connecting to host " << host_to_connect_ << ":" << port_ << " failed. Aborting.";
            emit host_connected(false);
        }
    }
}

void myClient::timeout_on_connection() {
    QString state;
    switch (previous_state_) {
        case QAbstractSocket::UnconnectedState:
            state = "initializing the socket";
            break;
        case QAbstractSocket::HostLookupState:
            state = "host lookup";
            break;
        case QAbstractSocket::ConnectingState:
            state = "ip connection to server";
            break;
        default:
            state = "unknown";
    }
    qWarning() << "Error occurred: " << state << " failed. Aborting connection";
    // emit host_connected(false);
}

void myClient::handle_ssl_handshake(const QList<QSslError>& errors) {
    qDebug() << "SSL ERRORS:";
    for (int i = 0; i < errors.size(); ++i) {
        qDebug() << "Error type: " << static_cast<int>(errors.at(i).error()) << "; error: " << errors.at(i).errorString();
    }
    socket->ignoreSslErrors();
}

void myClient::connection_encrypted() {
    qDebug() << "Connection to host " << host_to_connect_ << " succeeded and encrypted";
    emit host_connected(true);
}

void myClient::connect(const QString& ip_address, quint16 ip_port) {
    if (ip_address != "localhost") { // user inserted ip and port manually
        host_to_connect_ = ip_address;
        port_ = ip_port;
    } else {
        if (host_to_connect_.isEmpty()) { // not set by any callback
            host_to_connect_ = host_address_;
        }
        // otherwise set by the callback
    }
    connecting_interrupt_->start(5000);
    socket->connectToHost(host_to_connect_, port_);
    return;
}

void myClient::destroy_previous_connection() {
    host_to_connect_ = "";
    user.clear_fields();
//    QObject::disconnect(socket, &Socket::ready_message, this, &myClient::process_data_from_server);
//    QObject::connect(socket, &Socket::ready_message, this, &myClient::process_response);
}
void myClient::connect_response(){
    QObject::disconnect(socket, &Socket::ready_message, this, &myClient::process_data_from_server);
    QObject::connect(socket, &Socket::ready_message, this, &myClient::process_response);
}

void myClient::send_message(const QSharedPointer<Message>& request) {
    message_to_send_ = request;
    MessageType request_type = message_to_send_->type();
    if (request_type == MessageType::signup || request_type == MessageType::login || request_type == MessageType::profile ||
        request_type == MessageType::documents || request_type == MessageType::create || request_type == MessageType::open) {
        wait_on_connection_->start();
    }
    socket->write_message(request);
}

void myClient::attempt_timeout() {
    QString type;
    switch (message_to_send_->type()) {
        case MessageType::login:
            type = tr("login");
            break;
        case MessageType::signup:
            type = tr("signup");
            break;
        case MessageType::profile:
            type = tr("profile update");
            break;
        case MessageType::documents:
            type = tr("list of your documents");
            break;
        case MessageType::open:
            type = tr("open file");
            break;
        case MessageType::create:
            type = tr("create file");
            break;
        default:
            qWarning() << "A timeout should never fire for messages sent of type " << static_cast<int>(message_to_send_->type());
    }
    emit timeout_expired(type);
    return;
}

/*
 * The following function does process only messages which are a response to client's requests.
 * For "interactive" messages, once the document is opened/create, there is the 'process_data_from_server' function.
 */
void myClient::process_response() {
    bool timer_active = wait_on_connection_->isActive();
    // timer should not be active, but in the case we stop it to be sure
    if (timer_active) {
        int time_remaining = wait_on_connection_->remainingTime();
        wait_on_connection_->stop();
    }
    QSharedPointer<Message> response = socket->read_message();
    switch (message_to_send_->type()) {
        // following cases are for profile requests
        case MessageType::signup :
            if (response->type() != MessageType::signup_ok && response->type() != MessageType::error) {
                qWarning() << "Unsupported response message type. Expected 'signup_ok(2)/error(0)', received " << static_cast<int>(response->type());
                break;
            }
            if (response->type() == MessageType::signup_ok) {
                emit authentication_result(true, tr("Ok"));
            } else { // error
                user = UserInfo(); // just a way to clear local resident information
                emit authentication_result(false, response.staticCast<ErrorMessage>()->reason());
            }
            break;
        case MessageType::login :
            if (response->type() != MessageType::profile && response->type() != MessageType::error) {
                qWarning() << "Unsupported response message type. Expected 'profile(5)/error(0)', received " << static_cast<int>(response->type());
                break;
            }
            if (response->type() == MessageType::profile) {
                user = UserInfo(response.staticCast<ProfileMessage>()->profile());
                emit authentication_result(true, tr("Ok"));
            } else {
                user = UserInfo(); // just a way to clear local resident information
                emit authentication_result(false, response.staticCast<ErrorMessage>()->reason());
            }
            break;
        case MessageType::profile :
            // maybe here, maybe only in 'process_data_from_server'
            if (response->type() != MessageType::profile_ok && response->type() != MessageType::error) {
                qWarning() << "Unsupported response message type. Expected 'profile_ok(6)/error(0)', received" << static_cast<int>(response->type());
                break;
            }
            if (response->type() == MessageType::profile_ok) {
                user = new_user;
                emit profile_update_result(true, tr("Ok"));
                // to be connected to the slot of the new user widget (to manage all profile data)
            } else {
                emit profile_update_result(false, response.staticCast<ErrorMessage>()->reason());
            }
            break;
        case MessageType::documents :
            if (response->type() != MessageType::documents && response->type() != MessageType::error) {
                qWarning() << "Unsupported response message type. Expected 'documents(7)/error(0)', received " << static_cast<int>(response->type());
                break;
            }
            if (response->type() == MessageType::documents) {
                auto docs = response.staticCast<DocumentsMessage>()->documents();
                if (docs){ // user has a list of documents (maybe empty)
                    emit user_documents(docs.value());
                }
            } else {
                emit generic_error(tr("Received an error message while documents were expected.\n") + response.staticCast<ErrorMessage>()->reason());
            }
            break;
        case MessageType::open :
        case MessageType::create :
            if (response->type() != MessageType::document && response->type() != MessageType::error) {
                qWarning() << "Unsupported response message type. Expected 'document(11)/error(0)', received " << static_cast<int>(response->type());
                break;
            }
            if (response->type() == MessageType::document) {
                QObject::disconnect(socket, &Socket::ready_message, this, &myClient::process_response);
                QObject::connect(socket, &Socket::ready_message, this, &myClient::process_data_from_server);
                QSharedPointer<DocumentMessage> document_message = response.staticCast<DocumentMessage>();
                QString document_name = document_message->document().full_name();
                if ( ( (message_to_send_->type() == MessageType::create)  // file just created by user
                    || (message_to_send_->type() == MessageType::open && static_cast<OpenMessage*>(message_to_send_.get())->sharing_link()
                        && !(static_cast<OpenMessage*>(message_to_send_.get())->document()) ) // document opened by sharing link
                    ) && !user.filename_to_owner_map.contains(document_name) // be sure the document is not yet in the list
                    ) {
                    user.filename_to_owner_map.insert(document_name, document_message->document());
                }
                fileInfo file(document_message->document(), document_message->document_data());
                emit document(file);
            } else {
                emit generic_error(tr("Received an error message instead of document data.\n") + response.staticCast<ErrorMessage>()->reason());
            }
            break;
        default:
            // we may reach this point if disconnection from this slot and connection to the new one was later than new data coming from server
            qDebug() << "Message sent type " << static_cast<int>(message_to_send_->type()) << "not implemented now";
            qDebug() << "Received message of type " << static_cast<int>(response->type());
            qWarning() << "A generic error occurred";
    }
}

void myClient::process_data_from_server() {
    Profile other;
    QString username;
    QSharedPointer<OpenMessage> opened;
    QSharedPointer<CloseMessage> closed;
    QSharedPointer<InsertMessage> inserted;
    QSharedPointer<EraseMessage> erased;
    QSharedPointer<CursorMessage> cursor_update;
    int site_id;
    Symbol symbol;
    QSharedPointer<Message> response = socket->read_message();
    switch (response->type()) {
        case MessageType::error :
            emit generic_error(tr("The server returned the following error message.\n") + response.staticCast<ErrorMessage>()->reason());
            break;
        case MessageType::profile_ok :
            // update editor UI with new user data (maybe only password changed)
            user = new_user;
            emit profile_update_result(true, tr("Ok"));
            break;
        case MessageType::open :
            // add the user to user list
            opened = response.staticCast<OpenMessage>();
            other = opened->profile().value();
            site_id = opened->site_id().value();
            emit user_added(other, site_id);
            break;
        case MessageType::close :
            // remove user from user list
            closed = response.staticCast<CloseMessage>();
            username= closed->username().value();
            emit user_removed(username,0); // site_id from close message
            break;
        case MessageType::insert :
            // perform remote insert
            inserted = response.staticCast<InsertMessage>();
            symbol = inserted->symbol();
            emit char_inserted(symbol);
            break;
        case MessageType::erase :
            // perform remote erase
            erased = response.staticCast<EraseMessage>();
            symbol = erased->symbol();
            emit char_removed(symbol);
            break;
        case MessageType::cursor :
            // update the position of the cursor
            cursor_update = response.staticCast<CursorMessage>();
            symbol = cursor_update->symbol();
            username = cursor_update->username().value();
            emit cursor(symbol, username);
            break;
        default:
            qWarning() << "'Control' messages are not allowed while editing";
            qWarning() << "Received message of type " << static_cast<int>(response->type());
    }
}

void myClient::login(QString& email, QString& password) {
    QSharedPointer<Message> login_message = QSharedPointer<LoginMessage>::create(email, password);
    send_message(login_message);
}

void myClient::logout() {
   QSharedPointer<Message> logout_message = QSharedPointer<LogoutMessage>::create();
   user.clear_fields();
   send_message(logout_message);
}

void myClient::signup(QString& username, QString& email, QString& password, QString name, QString surname, QImage icon) {
    //TODO: add icon on signup
    Profile profile(username,name,surname,icon);
    user = UserInfo(profile);
    QSharedPointer<Message> signup_message = QSharedPointer<SignupMessage>::create(profile,password);
    send_message(signup_message);
}

void myClient::update_profile(const QString &username, const QString &email, const QString &name, const QString &surname, const QImage &icon, QString &password) {
    new_user = Profile(username, name, surname, icon);
    QSharedPointer<Message> profile_update_message;
    if (password.isEmpty()) {
        profile_update_message = QSharedPointer<ProfileMessage>::create(new_user);
    } else {
        profile_update_message = QSharedPointer<ProfileMessage>::create(new_user, password);
    }
    password.fill(0);
    password.clear();
    send_message(profile_update_message);
}

void myClient::sendInsert(const Document& document,const Symbol& s){
    QSharedPointer<Message> insert_message = QSharedPointer<InsertMessage>::create(document,s);
    send_message(insert_message);
}

void myClient::sendErase(const Document& document, const Symbol& s){
    QSharedPointer<Message> erase_message = QSharedPointer<EraseMessage>::create(document,s);
    send_message(erase_message);
}


void myClient::new_file(const QString& filename){
    QSharedPointer<Message> create_message =  QSharedPointer<CreateMessage>::create(filename);
    send_message(create_message);
}

void myClient::open_file(const QString& file, bool isFilename){
    QSharedPointer<Message> open_message;
    if (isFilename) {
        Document doc = user.filename_to_owner_map[file];
        open_message = QSharedPointer<OpenMessage>::create(doc);
    } else { // sharing link
        open_message = QSharedPointer<OpenMessage>::create(file);
    }
    send_message(open_message);
}

void myClient::file_close(const fileInfo& file){
    QSharedPointer<Message> close_message = QSharedPointer<CloseMessage>::create(file.document(), user.username());
    send_message(close_message);
    QObject::disconnect(socket, &Socket::ready_message, this, &myClient::process_data_from_server);
    QObject::connect(socket, &Socket::ready_message, this, &myClient::process_response);
}

void myClient::get_documents_form_server() {
    QSharedPointer<Message> documents_message = QSharedPointer<DocumentsMessage>::create();
    send_message(documents_message);
}

void myClient::send_cursor(Document document, Symbol cursor_position){
    QSharedPointer<Message> cursor_message = QSharedPointer<CursorMessage>::create(document, cursor_position);
    send_message(cursor_message);
}