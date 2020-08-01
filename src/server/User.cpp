/*
 * Author: Franco Ruggeri
 */

#include "User.h"

User::User(TcpSocket* socket) : socket_(socket), authenticated_(false) {}

TcpSocket* User::socket() const {
    return socket_;
}

QSharedPointer<Profile> User::profile() const {
    return profile_;
}

QSet<Document> User::accessible_documents() const {
    return accessible_documents_;
}

bool User::authenticated() const {
    return authenticated_;
}

void User::set_profile(const QSharedPointer<Profile>& profile) {
    profile_ = profile;
}

void User::set_accessible_documents(const QSet<Document>& accessible_documents) {
    accessible_documents_ = accessible_documents;
}

void User::set_authenticated(bool authenticated) {
    authenticated_ = authenticated;
}

//void User::create_document(const QString &document, QSharedPointer<SafeSharedEditor> local_editor) {
//    available_documents_.insert(document);
//    local_editors_.insert(document, local_editor);
//}
//
//void User::open_document(const QString &document, QSharedPointer<SafeSharedEditor> local_editor) {
//    local_editors_.insert(document, local_editor);
//}
//
//void User::close_document(const QString &document) {
//    local_editors_.remove(document);
//}

bool User::is_open(const Document& document) const {
    return open_documents_.contains(document);
}

//bool User::is_available(const QString &document) {
//    return available_documents_.contains(document);
//}

QSharedPointer<SafeSharedEditor> User::editor(const Document& document) const {
    auto it = open_documents_.find(document);
    if (it == open_documents_.end()) throw std::logic_error("document not found");
    return *it;
}
