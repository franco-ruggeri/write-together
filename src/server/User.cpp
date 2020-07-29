/*
 * Author: Franco Ruggeri
 */

#include "User.h"

User::User(const QSharedPointer<TcpSocket> &socket) : socket_(socket), authenticated_(false) {}

QSharedPointer<TcpSocket> User::socket() const {
    return socket_;
}

UserProfile& User::profile() {
    return profile_;
}

QList<QString> User::available_documents() {
    return available_documents_.toList();
}

bool User::authenticated() const {
    return authenticated_;
}

void User::set_profile(const UserProfile& profile) {
    profile_ = profile;
}

void User::set_available_documents(QSet<QString> available_documents) {
    available_documents_ = available_documents;
}

void User::set_authenticated(bool authenticated) {
    authenticated_ = authenticated;
}

void User::create_document(const QString &document, QSharedPointer<SafeSharedEditor> local_editor) {
    available_documents_.insert(document);
    local_editors_.insert(document, local_editor);
}

void User::open_document(const QString &document, QSharedPointer<SafeSharedEditor> local_editor) {
    local_editors_.insert(document, local_editor);
}

void User::close_document(const QString &document) {
    local_editors_.remove(document);
}

bool User::is_open(const QString &document) {
    return local_editors_.contains(document);
}

bool User::is_available(const QString &document) {
    return available_documents_.contains(document);
}

QSharedPointer<SafeSharedEditor> User::local_editor(const QString &document) {
    return local_editors_[document];
}

void User::clear() {
    profile_.clear();
    available_documents_.clear();
    local_editors_.clear();
    authenticated_ = false;
}