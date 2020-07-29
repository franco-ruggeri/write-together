/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <protocol/UserProfile.h>
#include <network/TcpSocket.h>
#include "SafeSharedEditor.h"

using namespace collaborative_text_editor;

class User {
    QSharedPointer<TcpSocket> socket_;
    UserProfile profile_;
    QSet<QString> available_documents_;                                 // owned or shared with this user
    QHash<QString,QSharedPointer<SafeSharedEditor>> local_editors_;     // local (dummy) editors
    bool authenticated_;

public:
    User(const QSharedPointer<TcpSocket>& socket);

    QSharedPointer<TcpSocket> socket() const;
    UserProfile& profile();
    QList<QString> available_documents();
    bool authenticated() const;
    void set_profile(const UserProfile& profile);
    void set_available_documents(QSet<QString> available_documents);
    void set_authenticated(bool authenticated);

    void create_document(const QString& document, QSharedPointer<SafeSharedEditor> local_editor);
    void open_document(const QString& document, QSharedPointer<SafeSharedEditor> local_editor);
    void close_document(const QString& document);
    bool is_open(const QString& document);
    bool is_available(const QString& document);
    QSharedPointer<SafeSharedEditor> local_editor(const QString& document);

    void clear();
};


