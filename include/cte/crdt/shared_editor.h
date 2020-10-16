/*
 * Local instance of editor shared with other users through the network. The communication on the network is not
 * handled here, it must be implemented outside.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QList>
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

    public:
        explicit SharedEditor(int site_id, const Lseq& pos_allocator=Lseq{});
        SharedEditor(int site_id, const QList<Symbol>& text, const Lseq& pos_allocator=Lseq{});

        Symbol local_insert(int index, QChar value);
        Symbol local_erase(int index);
        int remote_insert(const Symbol& symbol);
        std::optional<int> remote_erase(const Symbol& symbol);

        int find(const Symbol& symbol) const;   // returns lower bound index, non-existing symbol is ok
        Symbol at(int index) const;

        int site_id() const;
        QList<Symbol> text() const;
        QString to_string() const;

        static const int invalid_site_id, invalid_site_counter;
        static const int starting_site_id, starting_site_counter;
    };
}