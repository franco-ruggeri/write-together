/*
 * Author: Antonino Musmeci
 */

#include "client/fileInfo.h"


fileInfo::fileInfo(Document document, QVector<Symbol> file_content, QHash<QString,Profile> users): document(document),
file_content(file_content), users(users){

}

const QVector<Symbol> &fileInfo::getFileContent() const {
    return file_content;
}

const Document fileInfo::getDocument() const {
    return document;
}

const QHash<QString,Profile> &fileInfo::getUsers() const {
    return users;
}