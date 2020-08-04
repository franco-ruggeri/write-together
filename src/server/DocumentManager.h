/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <editor/crdt/Symbol.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/DocumentData.h>
#include <editor/protocol/Profile.h>

class DocumentManager {
    QHash<QString,QVector<editor::Document>> open_documents_;           // username -> open documents
    QHash<editor::Document,QHash<QString,editor::Symbol>> cursors_;     // open document -> {username -> symbol}
    QMutex mutex_;

public:
    std::optional<editor::DocumentData> create_document(const editor::Document& document);
    std::optional<editor::DocumentData> open_document(const editor::Document& document, const QString& username);
    void close_document(const editor::Document& document, const QString& username);
    void insert_symbol(const editor::Document& document, const editor::Symbol& symbol);
    void erase_symbol(const editor::Document& document, const editor::Symbol& symbol);
    void move_cursor(const editor::Document& document, const QString& username, const editor::Symbol& symbol);
};
