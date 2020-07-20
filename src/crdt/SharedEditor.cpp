/*
 * Author: Franco Ruggeri
 */

#include "SharedEditor.h"
#include "MessageType.h"
#include <sstream>

namespace collaborative_text_editor {
    void SharedEditor::local_insert(int index, char value) {
        // allocate position
        std::vector<int> prev_pos = index == 0 ? pos_allocator_.get_begin() : text_.at(index-1).position();
        std::vector<int> next_pos = index == text_.size() ? pos_allocator_.get_end() : text_.at(index).position();
        std::vector<int> between_pos = pos_allocator_.get_between(prev_pos, next_pos);

        // insert locally
        Symbol symbol(value, site_id_, site_counter_++, between_pos);
        text_.insert(text_.begin()+index, symbol);

        // send message to server
        Message message(site_id_, MessageType::insert, symbol);
        // TODO: send to server
    }

    void SharedEditor::local_erase(int index) {
        Symbol symbol = text_.at(index);
        text_.erase(text_.begin() + index);     // erase locally
        // TODO: send to server                         // send message to server
    }

    void SharedEditor::remote_insert(const Message &message) {
        Symbol symbol = message.symbol();
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        text_.insert(it, symbol);
    }

    void SharedEditor::remote_erase(const Message &message) {
        Symbol symbol = message.symbol();
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it != text_.end() && *it == symbol) text_.erase(it);
    }

    void SharedEditor::process(const Message& message) {
        switch (message.type()) {
            case MessageType::insert:
                remote_insert(message);
                break;
            case MessageType::erase:
                remote_erase(message);
                break;
            default:
                throw std::logic_error("invalid MessageType");
        }
    }

    std::string SharedEditor::to_string() {
        std::ostringstream oss;
        for (const auto& s : text_)
            oss << s.value();
        return oss.str();
    }
}