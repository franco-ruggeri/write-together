/*
 * Local instance of editor shared with other users through the network. The communication on the network is not
 * handled here, it must be implemented outside.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QChar>
#include <protocol/Message.h>
#include <crdt/Symbol.h>
#include <crdt/Lseq.h>

namespace collaborative_text_editor {
    class SharedEditor {
        int site_id_, site_counter_;
        std::vector<Symbol> text_;
        Lseq pos_allocator_;

    public:
        SharedEditor(int site_id);
        SharedEditor(int site_id, const Lseq& pos_allocator);
        Symbol local_insert(int index, QChar value);
        Symbol local_erase(int index);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        QString to_string();

        static const int invalid_site_id;
        static const int invalid_site_counter;
    };
}
