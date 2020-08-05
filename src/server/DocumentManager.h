/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QVector>
#include <QtCore/QMutex>
#include <cte/crdt/Symbol.h>
#include <cte/crdt/SafeSharedEditor.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/DocumentData.h>

class DocumentManager {
    typedef struct {
        QSharedPointer<cte::SafeSharedEditor> local_copy;    // pointer because SafeSharedEditor is not assignable
        int reference_count;
        int next_site_id;
        // TODO: cursors
    } open_document_t;

    QHash<cte::Document,open_document_t> open_documents_;
    QHash<int,QHash<cte::Document,int>> site_ids_;           // session_id -> {document -> site_id}
    QMutex mutex_;

    QSharedPointer<cte::SafeSharedEditor> local_copy(const cte::Document& document);
    bool site_id_spoofing(int session_id, const cte::Document& document, const cte::Symbol& symbol);

public:
    std::optional<cte::DocumentData> create_document(int session_id, const cte::Document& document);
    std::optional<cte::DocumentData> open_document(int session_id, const QString& username,
                                                   const cte::Document& document);
    void insert_symbol(int session_id, const cte::Document& document, const cte::Symbol& symbol);
    void erase_symbol(const cte::Document& document, const cte::Symbol& symbol);

    void save();
};
