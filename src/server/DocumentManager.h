/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QVector>
#include <QtCore/QMutex>
#include <editor/crdt/Symbol.h>
#include <editor/crdt/SafeSharedEditor.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/DocumentData.h>

class DocumentManager {
    // local dummy editors, dummy because they don't insert/erase, they just keep updating (necessary for save())
    QHash<editor::Document,QSharedPointer<editor::SafeSharedEditor>> local_copies_;

    // session_id -> documents, for open documents, to avoid multiple opens from same session
    QHash<int,QSet<editor::Document>> open_documents_;

    // all accesses in mutual exclusion (thread-safe)
    QMutex mutex_;

    QSharedPointer<editor::SafeSharedEditor> local_copy(const editor::Document& document);

public:
    std::optional<editor::DocumentData> create_document(int session_id, const QString& document_name);
    void insert_symbol(const editor::Document& document, const editor::Symbol& symbol);
    void erase_symbol(const editor::Document& document, const editor::Symbol& symbol);
    void save();
};
