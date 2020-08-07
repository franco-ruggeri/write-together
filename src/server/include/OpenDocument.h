/*
 * Document that is open and is being currently edited by at least one client.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <cte/crdt/SharedEditor.h>
#include <cte/crdt/Symbol.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/DocumentData.h>

class OpenDocument {
    QSharedPointer<cte::SharedEditor> local_editor_;    // pointer to have assignability, SharedEditor is not assignable
    QHash<int,cte::Symbol> cursors_;                    // site_id -> symbol, only for online users
    QMultiHash<int,QString> site_ids_;                  // site_id -> username, for all users with at least one character
    int next_site_id_;
    int reference_count_;

public:
    typedef struct {
        int index;
        QChar value;
        QString author;
    } character_t;

    OpenDocument();
    explicit OpenDocument(const QVector<character_t>& text);

    int open(const QString& username);      // returns the assigned site_id
    void close(int site_id);
    void insert_symbol(const cte::Symbol& symbol);
    void erase_symbol(const cte::Symbol& symbol);
    void move_cursor(int site_id, const cte::Symbol& symbol);

    QList<cte::Symbol> text() const;
    QHash<int,cte::Symbol> cursors() const;
    QHash<int,QString> site_ids() const;
    int reference_count() const;
};
