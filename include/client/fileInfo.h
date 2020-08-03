/*
 * Author: Antonino Musmeci
 */

#pragma once

#include "crdt/Symbol.h"
#include <QString>
#include <protocol/Profile.h>

using namespace collaborative_text_editor;
class fileInfo {
private:


    Document document_;
    QList<Symbol> file_content_;
    QHash<QString,Profile> users_;
    QHash<QString,int> site_ids_;
    QHash<QString,Symbol> connected_user_;
    QString sharing_link_;
    int site_id_;
    int site_counter_;
public:
    fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,
            QHash<QString,int> site_ids,QString sharing_link,QHash<QString,Symbol> connected_user, int site_id,int site_counter);

    const QList<Symbol> &getFileContent() const;

    const Document &document() const;

    const QHash<QString,Profile> &users() const;
    const QHash<QString,Symbol> &connected_user() const;
    const QString &sharing_link() const;
    int site_id();
    int site_counter();

    int site_counter() const;
};



