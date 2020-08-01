/*
 * Author: Antonino Musmeci
 */
//TODO: add user name

#include "client/myClient.h"
#include <QPixmap>
#include <QJsonDocument>
#include <protocol/OpenMessage.h>
#include <protocol/ProfileMessage.h>
#include <client/fileInfo.h>
#include <protocol/CloseMessage.h>
#include <protocol/CursorMessage.h>
#include <protocol/DocumentMessage.h>
#include "protocol/SignupMessage.h"
#include "protocol/LogoutMessage.h"
#include "protocol/InsertMessage.h"
#include "protocol/CreateMessage.h"
#include "protocol/EraseMessage.h"
#include "protocol/LoginMessage.h"
#include "protocol/ErrorMessage.h"
#include "protocol/DocumentsMessage.h"


using namespace collaborative_text_editor;
myClient::myClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
}

bool myClient::connect(QString ip_address) {
    socket->connectToHost(ip_address, PORT);
    return socket->waitForConnected(3000);
}

QSharedPointer<Message> myClient::send_message(const QSharedPointer<Message>& request) {
    QSharedPointer<Message> m;
    socket->write(request->serialize() + '\n');
    if(socket->waitForReadyRead(1000)) {
        QByteArray response = socket->readLine();
        m = Message::deserialize(response);
        return m;
    }
    return QSharedPointer<ErrorMessage>::create("TIME_OUT");
}



std::tuple<bool,QString> myClient::login(QString& email, QString& password) {
    QString result = "ERROR!";
    QSharedPointer<Message> login_message = QSharedPointer<LoginMessage>::create(email, password);
    QSharedPointer<Message> response = send_message(login_message);
    if(response ->type() == MessageType::profile){
        QSharedPointer<ProfileMessage> profile = response.staticCast<ProfileMessage>();
        user = profile->profile();
        //TODO: add user name

        return std::make_tuple(true,"OK");
    }
    if(response ->type() == MessageType::error){
        result = {response.staticCast<ErrorMessage>()->reason()};
        return std::make_tuple(false,result);
    }
    return std::make_tuple(false,result);

}

void myClient::logout() {
   QSharedPointer<Message> logout_message = QSharedPointer<LogoutMessage>::create();
    send_message(logout_message);
}

std::tuple<bool,QString> myClient::signup(QString& username, QString& email, QString& password) {
    QString result;
    QSharedPointer<Message> signup_message = QSharedPointer<SignupMessage>::create(username,password);
    QSharedPointer<Message> response = send_message(signup_message );
    if(response ->type() == MessageType::profile){
        auto profile = response.staticCast<ProfileMessage>();
        user = profile->profile();
        return std::make_tuple(true,"OK");
    }
    if(response ->type() == MessageType::error){
        result = response.staticCast<ErrorMessage>()->reason();
        return std::make_tuple(false,result);
    }
    return std::make_tuple(false,result);
}

void myClient::sendInsert(const Document& document,const Symbol& s){
    QSharedPointer<Message> insert_message = QSharedPointer<InsertMessage>::create(document,s);
    socket->write(insert_message->serialize() + '\n');
}

void myClient::sendErase(const Document& document, const Symbol& s){
    QSharedPointer<Message> erase_message = QSharedPointer<EraseMessage>::create(document,s);
    socket->write(erase_message->serialize() + '\n');
}

std::optional<fileInfo> myClient::new_file(const QString& filename){

    QSharedPointer<Message> create_message =  QSharedPointer<CreateMessage>::create(filename);
    QSharedPointer<Message> response  =  send_message(create_message);
    if(response -> type() == MessageType::document){
        QSharedPointer<DocumentMessage>document_info = response.staticCast<DocumentMessage>();
        return fileInfo(document_info->document(),document_info->text(),document_info->profiles(),
                        document_info->site_ids(),document_info->sharing_link(),document_info->cursors());
    }

    return std::nullopt;
}

fileInfo myClient::open_file(const QString& filename){

    QSharedPointer<Message> open_message = QSharedPointer<OpenMessage>::create(filename);
    QSharedPointer<Message> response = send_message(open_message);
    QSharedPointer<DocumentMessage>document_info = response.staticCast<DocumentMessage>();
    fileInfo file(document_info->document(),document_info->text(),document_info->profiles(),
            document_info->site_ids(),document_info->sharing_link(),document_info->cursors());
    return file;
}

bool myClient::change_password(const QString& new_password) {
    Profile profile = Profile(user.username(),user.name(),user.username(),user.icon());
    QSharedPointer<Message> profile_message = QSharedPointer<ProfileMessage>::create(profile,new_password);
    QSharedPointer<Message> response = send_message(profile_message);
    if(response -> type() == MessageType::error)
        return false;
    return response->type() == MessageType::profile_ok;

}

void myClient::file_close(const fileInfo& file){
    QSharedPointer<Message> close_message = QSharedPointer<CloseMessage>::create(file.document(), user.username());
    QSharedPointer<Message> response = send_message(close_message);
}


bool myClient::change_username(const QString& new_username){
    Profile profile = Profile(new_username,user.name(),user.username(),user.icon());
    QSharedPointer<Message> profile_message = QSharedPointer<ProfileMessage>::create(profile);
    QSharedPointer<Message> response = send_message(profile_message);
    if(response -> type() == MessageType::error)
        return false;
    return response->type() == MessageType::profile_ok;
}

//std::optional<QString> myClient::get_uri(const QString& filename){
//
//    /*************get uri from server*************/
//    return "uri_del_file_ritornato_dal_server";
//}

QList<Document> myClient::get_documents_form_server() {
    QSharedPointer<Message> documents_message = QSharedPointer<DocumentsMessage>::create();
    QSharedPointer<Message> response = send_message(documents_message);
    if(response->type() == MessageType::documents){
        return *response.staticCast<DocumentsMessage>()->documents();
    }
}


void myClient::send_cursor(Document document, Symbol cursor_position){
    QSharedPointer<Message> cursor_message = QSharedPointer<CursorMessage>::create(document, user.username(), cursor_position);
    socket->write(cursor_message->serialize() + '\n');

}