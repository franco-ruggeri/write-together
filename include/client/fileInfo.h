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
    QVector<Symbol> file_content;
    QHash<QString,Profile> users;
public:
    fileInfo(Document document, QVector<Symbol> file_content, QHash<QString,Profile> users);

    const QVector<Symbol> &getFileContent() const;

    const Document getDocument() const;

    const QHash<QString,Profile> &getUsers() const;

};



