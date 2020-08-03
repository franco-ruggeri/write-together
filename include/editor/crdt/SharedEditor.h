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
#include <editor/protocol/Message.h>
#include <editor/crdt/Symbol.h>
#include <editor/crdt/Lseq.h>

namespace editor {
    class SharedEditor {
        int site_id_, site_counter_;
        QList<Symbol> text_;
        Lseq pos_allocator_;

    public:
        SharedEditor(int site_id, int site_counter, const Lseq& pos_allocator=Lseq{});
        SharedEditor(int site_id, int site_counter, const QList<Symbol>& text, const Lseq& pos_allocator=Lseq{});

        Symbol local_insert(int index, QChar value);
        Symbol local_erase(int index);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        int find(const Symbol& symbol);

        int site_id() const;
        int site_counter() const;
        QList<Symbol> text() const;
        QString to_string() const;

        static const int invalid_site_id, invalid_site_counter;
        static const int start_site_id, start_site_counter;
    };
}
