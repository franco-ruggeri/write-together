/*
 * Author: Antonino Musmeci
 */

#include <cte/client/fileInfo.h>

#include <utility>


fileInfo::fileInfo(Document const &document, DocumentData const data):document_(document),documentData_(data){

}


const QList<Symbol> fileInfo::getFileContent() const {
    return documentData_.text();
}

Document fileInfo::document() const {
    return document_;
}

QHash<QString,Profile> fileInfo::users() const {
    return documentData_.profiles();
}

QHash<int,QString> fileInfo::site_ids() const {
    return documentData_.site_ids();
}

QUrl fileInfo::sharing_link() const {
    return documentData_.sharing_link();
}
int fileInfo::site_id() {
    return documentData_.site_id();
}

QHash<int,Symbol> fileInfo::cursors() const {
    return documentData_.cursors();
}


