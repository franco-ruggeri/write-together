/*
 * Author: Antonino Musmeci
 */
//TODO: add user name
//TODO add error message
//TODO add cursor message
//TODO add insert icon
#include <cte/client/myClient.h>
#include <QPixmap>
#include <QJsonDocument>
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
        QObject::connect(socket, &Socket::ready_message, wait_on_connection_.get(), &QTimer::stop);
        QObject::connect(socket, &Socket::ready_message, this, &myClient::process_response);
        QObject::connect(wait_on_connection_.get(), &QTimer::timeout, this, &myClient::attempt_timeout);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return;
    }
}

bool myClient::connect(QString ip_address) {
    socket->connectToHost(ip_address, PORT);
    return socket->waitForConnected(3000);
}

void myClient::send_message(const QSharedPointer<Message>& request) {
    message_to_send_ = request;
    MessageType request_type = message_to_send_->type();
    if (request_type == MessageType::signup || request_type == MessageType::login || request_type == MessageType::profile ||
        request_type == MessageType::documents || request_type == MessageType::create || request_type == MessageType::open) {
        connection_attempts_ = 1; // in case of multiple attempts when timeout occurs
        wait_on_connection_->start();
    }
    socket->write_message(request); // not message_to_send_, which may be deleted meanwhile
}

void myClient::attempt_timeout() {
    connection_attempts_--;
    if (connection_attempts_ > 0) {
        wait_on_connection_->start();
        socket->write_message(message_to_send_);
    } else {
        // TODO: differentiate all different signals to emit for different use cases
        switch (message_to_send_->type()) {
            case MessageType::login:
            case MessageType::signup:
                emit authentication_result(false, tr("Failing in contacting the server in order to verify user data"));
                break;
            default:
                emit generic_error(tr("Error management in timeout for message type ") + QString::number(static_cast<int>(message_to_send_->type())));
        }
    }
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
                user = new_user; // better way to update only profile inside user
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

// TODO: implement possible cases
void myClient::process_data_from_server() {
    QSharedPointer<Message> response = socket->read_message();
    switch (response->type()) {
        case MessageType::error :
            emit generic_error(tr("The server returned the following error message.\n") + response.staticCast<ErrorMessage>()->reason());
            break;
        case MessageType::profile_ok :
            // update editor UI with new user data (maybe only password changed
            break;
        case MessageType::open :
            // add the user to user list
            break;
        case MessageType::close :
            // remove user from user list
            break;
        case MessageType::insert :
            // perform remote insert
            break;
        case MessageType::erase :
            // perform remote erase
            break;
        case MessageType::cursor :
            // update the position of the cursor
            break;
        default:
            qWarning() << "'Control' messages are not allowed while editing";
            qWarning() << "Received message of type " << static_cast<int>(response->type());
    }
}

void myClient::login(QString& email, QString& password) {
    QString result = "ERROR!";
    QSharedPointer<Message> login_message = QSharedPointer<LoginMessage>::create(email, password);
    send_message(login_message);
}

void myClient::logout() {
   QSharedPointer<Message> logout_message = QSharedPointer<LogoutMessage>::create();
    send_message(logout_message);
}

void myClient::signup(QString& username, QString& email, QString& password, QString name, QString surname) {
    QString result;
    //TODO: add icon on signup
    QImage icon = QPixmap(1, 1).toImage();
    Profile profile(username,name,surname,icon);
    user = UserInfo(profile);
    QSharedPointer<Message> signup_message = QSharedPointer<SignupMessage>::create(profile,password);
    send_message(signup_message );
}

void myClient::sendInsert(const Document& document,const Symbol& s){
    QSharedPointer<Message> insert_message = QSharedPointer<InsertMessage>::create(document,s);
    socket->write_message(insert_message);
}

void myClient::sendErase(const Document& document, const Symbol& s){
    QSharedPointer<Message> erase_message = QSharedPointer<EraseMessage>::create(document,s);
    socket->write_message(erase_message);
}


void myClient::new_file(const QString& filename){
    QSharedPointer<Message> create_message =  QSharedPointer<CreateMessage>::create(filename);
    send_message(create_message);
}

void myClient::open_file(const QString& filename){
// TODO: fix open message, which makes server crash
    qDebug() << filename;
    Document doc = user.filename_to_owner_map[filename];
    qDebug() << doc.full_name();
    QSharedPointer<Message> open_message = QSharedPointer<OpenMessage>::create(doc);
    send_message(open_message);
}

bool myClient::change_password(const QString& new_password) {
    Profile profile = Profile(user.username(),user.name(),user.surname(),user.icon());
    new_user = UserInfo(profile);
    QSharedPointer<Message> profile_message = QSharedPointer<ProfileMessage>::create(profile,new_password);
    send_message(profile_message);
    return false;
}

void myClient::file_close(const fileInfo& file){
    QSharedPointer<Message> close_message = QSharedPointer<CloseMessage>::create(file.document(), user.username());
    send_message(close_message);
}


bool myClient::change_username(const QString& new_username){
    Profile profile = Profile(new_username,user.name(),user.username(),user.icon());
    new_user = UserInfo(profile);
    QSharedPointer<Message> profile_message = QSharedPointer<ProfileMessage>::create(profile);
    send_message(profile_message);
    return false;
}


void myClient::get_documents_form_server() {
    QSharedPointer<Message> documents_message = QSharedPointer<DocumentsMessage>::create();
    send_message(documents_message);
}


void myClient::send_cursor(Document document, Symbol cursor_position){
    QSharedPointer<Message> cursor_message = QSharedPointer<CursorMessage>::create(document, cursor_position);
    socket->write_message(cursor_message);
}