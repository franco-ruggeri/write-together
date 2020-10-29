/*
 * Document that is open and is being currently edited by at least one client.
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>

namespace cte {
    class OpenDocument {
        QSharedPointer<SharedEditor> shared_editor_;    // pointer to have assignability, SharedEditor is not assignable
        QHash<int,Symbol> cursors_;                     // site_id -> symbol, for online users
        QHash<int,QString> usernames_;                  // site_id -> username, for all users
        int next_site_id_;
        int reference_count_;

    public:
        typedef struct {
            int index;
            QChar value;
            QString author;
        } character_t;

        OpenDocument();
        OpenDocument(const QVector<character_t>& text, const QList<QString>& usernames);

        int open(const QString& username);      // returns the assigned site_id
        void close(int site_id);
        void insert_symbol(const Symbol& symbol);
        void erase_symbol(int site_id, const Symbol& symbol);
        void move_cursor(int site_id, const Symbol& symbol);

        QList<Symbol> text() const;
        QString username(int site_id) const;
        QHash<int,Symbol> cursors() const;
        QHash<int,QString> usernames() const;
        int reference_count() const;
    };
}
