/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <QtCore/QString>
#include <cte/crdt/Symbol.h>
#include <cte/protocol/Profile.h>
#include <cte/protocol/DocumentData.h>
#include <cte/protocol/Document.h>

using namespace cte;
class fileInfo {
private:

    Document document_;
//    QList<Symbol> file_content_;
//    QHash<QString,Profile> users_;
//    QHash<int,QString> site_ids_;
//    QHash<int,Symbol> connected_user_;
//    QUrl sharing_link_;
//    int site_id_;
    DocumentData documentData_;
//    int site_counter_;




public:
//    fileInfo(Document document, QList<Symbol> file_content, QHash<QString,Profile> users,
//             QHash<int,QString> site_ids,QUrl sharing_link,QHash<int,Symbol> connected_user, int site_id,int site_counter);
//    fileInfo(Document const& document, DocumentData const &data);

    const QList<Symbol> getFileContent() const;

    Document document() const;

    QHash<QString,Profile> users() const;
    QHash<int,QString> connected_user() const;
    QUrl sharing_link() const;
    int site_id();
//    int site_counter();
//
//    int site_counter() const;
    QHash<int,Symbol> cursors() const;
    fileInfo(const Document &document, const DocumentData data);
};



