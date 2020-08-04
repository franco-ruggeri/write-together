/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QHash>
#include <editor/crdt/Symbol.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/Profile.h>

using DocumentData = std::tuple<
        QList<editor::Symbol>,              // text
        int,                                // site_id assigned to the current user
        int,                                // starting site_counter assigned to the current user
        QHash<QString, int>,                // site_ids of other users with access to the document
        QHash<QString, editor::Profile>,    // profiles of other users with access to the document
        QString>;                           // sharing link

class DocumentManager {
    QHash<editor::Document,int> open_documents;

public:
    DocumentManager();
    std::optional<DocumentData> create_document(const editor::Document& document);
    std::optional<DocumentData> open_document(const QString &username, const editor::Document &document);
    void close_document(const QString& username, const editor::Document& document);
    void insert_symbol(const editor::Document& document, const editor::Symbol& symbol);
    void erase_symbol(const editor::Document& document, const editor::Symbol& symbol);
};
