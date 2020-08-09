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


using namespace cte;
myClient::myClient(QObject *parent) : QObject(parent) {
    try {
        socket = new Socket();
    } catch (const std::exception &e) {
        qDebug() << e.what();
        return;
    }
}

bool myClient::connect(QString ip_address) {
    socket->connectToHost(ip_address, PORT);
    return socket->waitForConnected(3000);
}

QSharedPointer<Message> myClient::send_message(const QSharedPointer<Message>& request) {
    QSharedPointer<Message> m;
    socket->write_message(request);
    if(socket->waitForReadyRead(1000)) {
        return socket->read_message();
    }
    return QSharedPointer<ErrorMessage>::create("TIME_OUT");
}

std::tuple<bool,QString> myClient::login(QString& email, QString& password) {
    QString result = "ERROR!";
    QSharedPointer<Message> login_message = QSharedPointer<LoginMessage>::create(email, password);
    QSharedPointer<Message> response = send_message(login_message);

    if(response ->type() == MessageType::profile){
        QSharedPointer<ProfileMessage> profile = response.staticCast<ProfileMessage>();
        user = UserInfo(profile->profile());
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

std::tuple<bool,QString> myClient::signup(QString& username, QString& email, QString& password, QString name, QString surname) {
    QString result;
    //TODO: add icon on signup
    QImage icon = QPixmap(1, 1).toImage();
    Profile profile(username,name,surname,icon);
    QSharedPointer<Message> signup_message = QSharedPointer<SignupMessage>::create(profile,password);
    QSharedPointer<Message> response = send_message(signup_message );
    if(response ->type() == MessageType::signup_ok){
        user = profile;
        return std::make_tuple(true,"OK");
    }
    if(response ->type() == MessageType::error){
        result = response.staticCast<ErrorMessage>()->reason();
        return std::make_tuple(false,result);
    }
    return std::make_tuple(false,result);
}

void myClient::sendInsert(const Document& document,const Symbol& s){
    qDebug() << document.name();
    qDebug() << document.full_name();
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
        return fileInfo(document_info->document(),document_info->document_data());
    }
    return std::nullopt;
}

fileInfo myClient::open_file(const QString& filename){

    qDebug() << filename;
    Document doc = user.filename_to_owner_map[filename];
    qDebug() << doc.full_name();
    QSharedPointer<Message> open_message = QSharedPointer<OpenMessage>::create(doc);
    QSharedPointer<Message> response = send_message(open_message);
    if(response ->type() == MessageType::document) {
        QSharedPointer<DocumentMessage> document_info = response.staticCast<DocumentMessage>();
        fileInfo file(document_info->document(), document_info->document_data());
        return file;
    }
    //TODO add error message
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


QSet<Document> myClient::get_documents_form_server() {
    QSharedPointer<Message> documents_message = QSharedPointer<DocumentsMessage>::create();
    QSharedPointer<Message> response = send_message(documents_message);
    if(response->type() == MessageType::documents){
        return *response.staticCast<DocumentsMessage>()->documents();

    }
}


void myClient::send_cursor(Document document, Symbol cursor_position){
//    QSharedPointer<Message> cursor_message = QSharedPointer<CursorMessage>::create(document, user.username(), cursor_position);
//    socket->write(cursor_message->serialize() + '\n');
    //TODO add cursor message
}