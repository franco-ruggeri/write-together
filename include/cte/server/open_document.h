/*
 * Document that is open and is being currently edited by at least one client. All the operations are thread-safe,
 * executed in mutual exclusion.
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>
#include <cte/protocol/format.h>

namespace cte {
    class OpenDocument {
        Document document_;
        SharedEditor shared_editor_;
        QHash<int,Symbol> cursors_;                     // site_id -> symbol, for online users
        QHash<int,QString> usernames_;                  // site_id -> username, for all users
        QList<Format> formats_;                         // format for each symbol, aligned with shared_editor_->text()
        int next_site_id_;
        int reference_count_;
        bool changed_;                                  // changed since last save
        mutable QMutex mutex_;

    public:
        explicit OpenDocument(Document document);
        OpenDocument(const OpenDocument& other);
        OpenDocument& operator=(const OpenDocument& other);

        int open(const QString& username);              // returns the assigned site_id
        void close(int site_id);
        bool save();                                    // returns true if actually saved (i.e. changed since last save)

        void insert_symbol(const Symbol& symbol, const Format& format);
        void erase_symbol(int site_id, const Symbol& symbol);
        void move_cursor(int site_id, const Symbol& symbol);
        void format_symbol(const Symbol& symbol, const Format& format);

        Document document() const;
        QList<std::pair<Symbol,Format>> text() const;
        QHash<int,Symbol> cursors() const;
        QHash<int,QString> usernames() const;
        int reference_count() const;
        bool changed() const;
        void set_changed(bool changed);
    };
}
