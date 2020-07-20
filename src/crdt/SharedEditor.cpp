/*
 * Author: Franco Ruggeri
 */

#include "SharedEditor.h"
#include "MessageType.h"
#include <sstream>

namespace collaborative_text_editor {
    SharedEditor::SharedEditor(int site_id) : site_id_(site_id) {}

    Symbol SharedEditor::local_insert(int index, char value) {
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

    std::string SharedEditor::to_string() {
        std::ostringstream oss;
        for (const auto& s : text_)
            oss << s.value();
        return oss.str();
    }
}