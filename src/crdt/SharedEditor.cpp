/*
 * Author: Franco Ruggeri
 */

#include "SharedEditor.h"

namespace collaborative_text_editor {
    SharedEditor::SharedEditor(int site_id) : site_id_(site_id) {}

    Symbol SharedEditor::local_insert(int index, QChar value) {
        // allocate position
        std::vector<int> prev_pos = index == 0 ? pos_allocator_.get_begin() : text_.at(index-1).position();
        std::vector<int> next_pos = index == text_.size() ? pos_allocator_.get_end() : text_.at(index).position();
        std::vector<int> between_pos = pos_allocator_.get_between(prev_pos, next_pos);

        // insert locally
        Symbol symbol(value, site_id_, site_counter_++, between_pos);
        text_.insert(text_.begin()+index, symbol);
        return symbol;
    }

    Symbol SharedEditor::local_erase(int index) {
        Symbol symbol = text_.at(index);
        text_.erase(text_.begin() + index);
        return symbol;
    }

    void SharedEditor::remote_insert(const Symbol& symbol) {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        text_.insert(it, symbol);
    }

    void SharedEditor::remote_erase(const Symbol& symbol) {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it != text_.end() && *it == symbol) text_.erase(it);
    }

    QString SharedEditor::to_string() {
        QString result;
        for (const auto& s : text_)
            result.append(s.value());
        return result;
    }
}