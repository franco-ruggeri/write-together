/*
 * Author: Antonino Musmeci
 */

#include <cte/client/fileInfo.h>

#include <utility>


fileInfo::fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,
                    QHash<int,QString> site_ids,QUrl sharing_link,QHash<int,Symbol> connected_user, int site_id,int site_counter):
                    document_(std::move(document)),
file_content_(std::move(file_content)), users_(std::move(users)),
    site_ids_(std::move( site_ids)) ,sharing_link_(sharing_link), connected_user_(connected_user),site_id_(site_id), site_counter_(site_counter){

}
fileInfo::fileInfo(Document const &document, DocumentData const &data):document_(document) {
    site_id_ = data.site_id();
    file_content_ = data.text();
    users_ = data.profiles();
    connected_user_ = data.cursors();
    sharing_link_ = data.sharing_link();
//    site_counter_ = data.site_counter_user();
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

const QHash<int,Symbol> &fileInfo::connected_user() const {
    return connected_user_;
}

const QUrl &fileInfo::sharing_link() const {
    return sharing_link_;
}
int fileInfo::site_id() {
    return site_id_;
}

int fileInfo::site_counter() {
    return site_counter_;
}


