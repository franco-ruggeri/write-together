/*
 * Document manager. It provides an interface for creating, opening, closing, and editing documents.
 *
 * A document can be opened only once in a session. Note that it is not thread-safe to use the same session from
 * different threads.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QUrl>
#include <cte/server/open_document.h>
#include <cte/crdt/symbol.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>
#include <cte/protocol/profile.h>
#include <optional>

namespace cte {
    class DocumentManager {
        QHash<Document,OpenDocument> open_documents_;
        QHash<int,QHash<Document,int>> site_ids_;          // session_id -> {document -> site_id}
        mutable QMutex mutex_;

        OpenDocument& get_open_document(const Document& document);
        bool opened(int session_id, const Document& document) const;
        bool site_id_spoofing(int session_id, const Document& document, const Symbol& symbol) const;

    public:
        DocumentManager();

        // document management
        std::optional<DocumentInfo> create_document(int session_id, const Document& document);
        std::optional<DocumentInfo> open_document(int session_id, const Document& document, const QString& username);
        std::pair<Document,std::optional<DocumentInfo>> open_document(int session_id, const QUrl& sharing_link,
                                                                      const QString& username);
        int close_document(int session_id, const Document& document);
        QList<Document> get_document_list(const QString& username) const;
        QList<Document> get_open_documents(int session_id) const;

        // document editing
        void insert_symbol(int session_id, const Document& document, const Symbol& symbol);
        void erase_symbol(int session_id, const Document& document, const Symbol& symbol);
        int move_cursor(int session_id, const Document& document, const Symbol& symbol);
        void save();
    };
}
