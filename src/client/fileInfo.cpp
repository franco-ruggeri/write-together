/*
 * Author: Antonino Musmeci
 */

#include <cte/client/fileInfo.h>

#include <utility>

//
//fileInfo::fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,
//                    QHash<int,QString> site_ids,QUrl sharing_link,QHash<int,Symbol> connected_user, int site_id,int site_counter):
//                    document_(std::move(document)),
//file_content_(std::move(file_content)), users_(std::move(users)),
//    site_ids_(std::move( site_ids)) ,sharing_link_(sharing_link), connected_user_(connected_user),site_id_(site_id){
//
//}
//fileInfo::fileInfo(Document const &document, DocumentData const &data):document_(document) {
//    site_id_ = data.site_id();
//    file_content_ = data.text();
//    users_ = data.profiles();
//    connected_user_ = data.cursors();
//    sharing_link_ = data.sharing_link();
////    site_counter_ = data.site_counter_user();
//}
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


