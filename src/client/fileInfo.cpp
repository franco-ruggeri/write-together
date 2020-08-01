/*
 * Author: Antonino Musmeci
 */

#include "client/fileInfo.h"

#include <utility>


fileInfo::fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,QHash<QString,int> site_ids): document(std::move(document)),
file_content(std::move(file_content)), users(std::move(users)), site_ids(std::move( site_ids)){

}

const QList<Symbol> &fileInfo::getFileContent() const {
    return file_content;
}

Document fileInfo::getDocument() const {
    return document;
}

const QHash<QString,Profile> &fileInfo::getUsers() const {
    return users;
}