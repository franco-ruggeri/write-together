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


    Document document;
    QList<Symbol> file_content;
    QHash<QString,Profile> users;
    QHash<QString,int> site_ids;
public:
    fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users, QHash<QString,int> site_ids);

    const QList<Symbol> &getFileContent() const;

    Document getDocument() const;

    const QHash<QString,Profile> &getUsers() const;

};



