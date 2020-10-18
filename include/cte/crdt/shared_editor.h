/*
 * Local instance of editor shared with other users through the network. The communication on the network is not
 * handled here, it must be implemented outside.
 *
 * This editor uses CRDT (Conflict-free Replicated Data Type) and Version Vector (+ deletion buffer), guaranteeing:
 * - Commutativity of insert and erase operations.
 * - Idempotency of operations.
 * - Protection against out-of-order messages (can happen in peer-to-peer architecture or in multi-threaded server).
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QChar>
#include <cte/protocol/message.h>
#include <cte/crdt/symbol.h>
#include <cte/crdt/lseq.h>

namespace cte {
    class SharedEditor : public QObject {
        Q_OBJECT

        int site_id_;
        int site_counter_;
        QList<Symbol> text_;
        Lseq pos_allocator_;
        QHash<int,int> version_vector_;
        QList<Symbol> deletion_buffer_;

        void update_version_vector(const Symbol& symbol);
        std::optional<int> buffered_erase(const Symbol& symbol);

    signals:
        void remote_insert(int index, QChar value);
        void remote_erase(int index);

    public:
        explicit SharedEditor(int site_id, QObject *parent=nullptr);
        SharedEditor(int site_id, const QList<Symbol>& text, QObject *parent=nullptr);

        Symbol insert(int site_id, int site_counter, int index, QChar value);   // should be used only by the server
        Symbol local_insert(int index, QChar value);
        Symbol local_erase(int index);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);    // erase may happen later (put in deletion buffer)

        int find(const Symbol& symbol) const;       // returns lower bound index, non-existing symbol is ok
        Symbol at(int index) const;

        int site_id() const;
        QList<Symbol> text() const;
        QString to_string() const;

        static const int invalid_site_id, invalid_site_counter;
        static const int starting_site_id, starting_site_counter;
    };
}