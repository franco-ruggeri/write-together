/*
 * Local instance of editor shared with other users through the network. The communication on the network is not
 * handled here, it must be implemented outside.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QChar>
#include "Message.h"
#include "Symbol.h"
#include "LSEQ.h"

namespace collaborative_text_editor {
    class SharedEditor {
        int site_id_, site_counter_;
        std::vector<Symbol> text_;
        LSEQ pos_allocator_;

    public:
        SharedEditor(int site_id);
        SharedEditor(int site_id, const LSEQ& pos_allocator);
        Symbol local_insert(int index, QChar value);
        Symbol local_erase(int index);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        QString to_string();
    };
}
