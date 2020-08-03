/*
 * Author: Antonino Musmeci
 */

#include "client/fileInfo.h"

#include <utility>


fileInfo::fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,
                    QHash<QString,int> site_ids,QString sharing_link,QHash<QString,Symbol> connected_user, int site_id,int site_counter): document_(std::move(document)),
file_content_(std::move(file_content)), users_(std::move(users)),
    site_ids_(std::move( site_ids)) ,sharing_link_(sharing_link), connected_user_(connected_user),site_id_(site_id), site_counter_(site_counter){

}

const QList<Symbol> &fileInfo::getFileContent() const {
    return file_content_;
}

const Document &fileInfo::document() const {
    return document_;
}

const QHash<QString,Profile> &fileInfo::users() const {
    return users_;
}

const QHash<QString,Symbol> &fileInfo::connected_user() const {
    return connected_user_;
}

const QString &fileInfo::sharing_link() const {
    return sharing_link_;
}
int fileInfo::site_id() {
    return site_id_;
}

int fileInfo::site_counter() {
    return site_counter_;
}