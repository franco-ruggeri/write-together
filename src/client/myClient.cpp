/*
 * Author: Antonino Musmeci
 */


#include "client/myClient.h"
#include <QJsonDocument>
#include <protocol/OpenMessage.h>
#include <protocol/TextMessage.h>
#include "protocol/SignUpMessage.h"
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


std::shared_ptr<Message> myClient::send_message(const std::shared_ptr<Message>& request) {
//    QByteArray a = request->serialize();
    std::shared_ptr<Message> m;
    socket->write(request->serialize() + '\n');
    if(socket->waitForReadyRead(3000)) {
        QByteArray response = socket->readLine();
        m = Message::deserialize(response);
        return m;
    }

    return std::make_shared<ErrorMessage>("TIME_OUT");
}

std::tuple<bool,std::vector<QString>> myClient::login(QString& email, QString& password) {
    std::vector<QString> result;
    std::shared_ptr<Message> login_message = std::make_shared<LoginMessage>(email, password);
    std::shared_ptr<Message> response = send_message(login_message);
    if(response ->type() == MessageType::documents){
       result = std::static_pointer_cast<DocumentsMessage>(response)->documents();
       return std::make_tuple(true,result);
    }
    if(response ->type() == MessageType::error){
        result = {std::static_pointer_cast<ErrorMessage>(response)->reason()};
        return std::make_tuple(false,result);
    }
    return std::make_tuple(false,result);

}

void myClient::logout() {
    std::shared_ptr<Message> logout_message = std::make_shared<LogoutMessage>();
//    std::shared_ptr<Message> response = send_message(logout_message);
    send_message(logout_message);
}

std::tuple<bool,std::vector<QString>> myClient::signup(QString& username, QString& email, QString& password) {

    std::vector<QString> result;
    std::shared_ptr<Message> signup_message = std::make_shared<SignUpMessage>(username,password);
    std::shared_ptr<Message> response = send_message(signup_message );
    if(response ->type() == MessageType::documents){
        result = std::static_pointer_cast<DocumentsMessage>(response)->documents();
        return std::make_tuple(true,result);
    }
    if(response ->type() == MessageType::error){
        result = {std::static_pointer_cast<ErrorMessage>(response)->reason()};
        return std::make_tuple(false,result);
    }
    return std::make_tuple(false,result);
}

void myClient::sendInsert(QString filename,Symbol s){
    std::shared_ptr<Message> insert_message = std::make_shared<InsertMessage>(filename,s);
    send_message(insert_message);
}


void myClient::sendErase(QString filename, Symbol s){
    std::shared_ptr<Message> erase_message = std::make_shared<EraseMessage>(filename,s);
    send_message(erase_message);
}

void myClient::new_file(QString filename){
    std::shared_ptr<Message> create_message = std::make_shared<CreateMessage>(filename);
    send_message(create_message);
}

std::tuple<bool,QString> myClient::open_file(QString filename){
    std::shared_ptr<Message> open_message = std::make_shared<OpenMessage>(filename);
    std::shared_ptr<Message> response = send_message(open_message);
    QString text;
    if(response ->type() == MessageType::text){
        text = std::static_pointer_cast<TextMessage>(response)->text();
        qDebug() << text;
        return std::make_tuple(true,text);
    }
    if(response ->type() == MessageType::error){
        text = {std::static_pointer_cast<ErrorMessage>(response)->reason()};
        return std::make_tuple(false,text);
    }
    return std::make_tuple(false,text);
}