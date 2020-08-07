/*
 * Document manager. It provides an interface for creating, opening, closing, and editing documents.
 *
 * A document can be opened only once in a session. Note that it is not thread-safe to use the same session from
 * different threads.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include "OpenDocument.h"
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QUrl>
#include <cte/crdt/Symbol.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/DocumentData.h>
#include <cte/protocol/Profile.h>

class DocumentManager {
    QHash<cte::Document,OpenDocument> open_documents_;
    QHash<int,QHash<cte::Document,int>> site_ids_;          // session_id -> {document -> site_id}
    mutable QMutex mutex_;

    OpenDocument& get_open_document(const cte::Document& document);
    bool opened(int session_id, const cte::Document& document) const;
    bool site_id_spoofing(int session_id, const cte::Document& document, const cte::Symbol& symbol) const;

public:
    DocumentManager();
    std::optional<cte::DocumentData> create_document(int session_id, const cte::Document& document);
    std::optional<cte::DocumentData> open_document(int session_id, const cte::Document& document, const QString& username);
    void insert_symbol(int session_id, const cte::Document& document, const cte::Symbol& symbol);
    void erase_symbol(const cte::Document& document, const cte::Symbol& symbol);

    void save() const;
    cte::Document document(const QUrl& sharing_link) const;
};
