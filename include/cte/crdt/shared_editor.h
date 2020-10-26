/*
 * Local instance of editor shared with other users through the network. The communication on the network is not
 * handled here, it must be implemented outside.
 *
 * This editor uses CRDT (Conflict-free Replicated Data Type) and Version Vector (+ deletion buffer), guaranteeing:
 * - Commutativity of insert and erase operations.
 * - Idempotency of operations.
 * - Protection against out-of-order messages (can happen in peer-to-peer architecture or in multi-threaded server).
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QChar>
#include <cte/protocol/message.h>
#include <cte/crdt/symbol.h>
#include <cte/crdt/lseq.h>
#include <optional>

namespace cte {
    class SharedEditor {
        int site_id_;
        int site_counter_;
        QList<Symbol> text_;
        Lseq pos_allocator_;
        QHash<int,int> version_vector_;
        QList<Symbol> deletion_buffer_;

        static const int reserved_site_id;

        bool valid_index(int index) const;
        void update_version_vector(const Symbol& symbol);
        std::optional<int> process_deletion_buffer();               // returns index if erase is executed

    public:
        explicit SharedEditor(int site_id, QObject *parent=nullptr);
        SharedEditor(int site_id, const QList<Symbol>& text);

        Symbol insert(int site_id, int site_counter, int index, QChar value);   // should be used only by the server
        Symbol local_insert(int index, QChar value);            // index does not consider BOF and EOF
        Symbol local_erase(int index);                          // index does not consider BOF and EOF
        std::optional<int> remote_insert(const Symbol& symbol); // returns nullopt in case of erase in deletion buffer
        std::optional<int> remote_erase(const Symbol& symbol);  // returns nullopt if erase is put in deletion buffer

        int find(const Symbol& symbol) const;   // returns position <= symbol.position(), so non-existing symbol is ok
        Symbol at(int index) const;             // index considers BOF and EOF (e.g. you can retrieve BOF with index=0)

        int site_id() const;
        QList<Symbol> text() const;     // without BOF and EOF
        QString to_string() const;      // without BOF and EOF

        static const int invalid_site_id, invalid_site_counter;
        static const int starting_site_id, starting_site_counter;
        static const Symbol bof, eof;
    };
}