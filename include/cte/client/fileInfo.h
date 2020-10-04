/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <QtCore/QString>
#include <cte/crdt/Symbol.h>
#include <cte/protocol/Profile.h>
#include <cte/protocol/DocumentInfo.h>
#include <cte/protocol/Document.h>

using namespace cte;
class fileInfo {
private:

    Document document_;
    DocumentInfo documentData_;
    QHash<int,QString> site_ids_;

public:


    const QList<Symbol> getFileContent() const;

    Document document() const;

    QHash<QString,Profile> users() const;
    QHash<int,QString> site_ids() const;
    QUrl sharing_link() const;
    int site_id();

    QHash<int,Symbol> cursors() const;
    fileInfo(const Document &document, const DocumentInfo data);

    void insert_new_userId(int id, QString Username);
};



