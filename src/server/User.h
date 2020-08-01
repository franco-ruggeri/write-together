/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QList>
#include <protocol/Profile.h>
#include <protocol/Document.h>
#include <network/TcpSocket.h>
#include "SafeSharedEditor.h"

using namespace collaborative_text_editor;

class User {
    TcpSocket* socket_;
    QSharedPointer<Profile> profile_;
    QHash<Document,QSharedPointer<SafeSharedEditor>> open_documents_;
    QSet<Document> accessible_documents_;
    bool authenticated_;

public:
    User(TcpSocket* socket);

    TcpSocket* socket() const;
    QSharedPointer<Profile> profile() const;
    QSet<Document> accessible_documents() const;
    bool authenticated() const;

    void set_profile(const QSharedPointer<Profile>& profile);
    void set_accessible_documents(const QSet<Document>& accessible_documents);
    void set_authenticated(bool authenticated);

//    void create_document(const QString& document, QSharedPointer<SafeSharedEditor> local_editor);
//    void open_document(const QString& document, QSharedPointer<SafeSharedEditor> local_editor);
//    void close_document(const QString& document);
//    bool is_open(const QString& document);
//    bool is_available(const QString& document);
//    QSharedPointer<SafeSharedEditor> local_editor(const QString& document);
};


