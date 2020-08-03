/*
 * Author: Franco Ruggeri
 */

#include <editor/crdt/SharedEditor.h>
#include <algorithm>

namespace editor {
    const int SharedEditor::invalid_site_id = -1;
    const int SharedEditor::invalid_site_counter = -1;
    const int SharedEditor::start_site_id = 0;
    const int SharedEditor::start_site_counter = 0;

    SharedEditor::SharedEditor(int site_id, int site_counter, const Lseq& pos_allocator) :
        site_id_(site_id), site_counter_(site_counter), pos_allocator_(pos_allocator) {}

    SharedEditor::SharedEditor(int site_id, int site_counter, const QList<Symbol>& text, const Lseq& pos_allocator) :
        site_id_(site_id), site_counter_(site_counter), text_(text), pos_allocator_(pos_allocator) {}

    Symbol SharedEditor::local_insert(int index, QChar value) {
        // allocate position
        QVector<int> prev_pos = index == 0 ? pos_allocator_.begin() : text_.at(index-1).position();
        QVector<int> next_pos = index == text_.size() ? pos_allocator_.end() : text_.at(index).position();
        QVector<int> between_pos = pos_allocator_.between(prev_pos, next_pos);

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

    int SharedEditor::find(const Symbol& symbol) {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it != text_.end() || *it == symbol) return std::distance(text_.begin(), it);
        else throw std::logic_error("symbol not found");
    }

    int SharedEditor::site_id() const {
        return site_id_;
    }

    int SharedEditor::site_counter() const {
        return site_counter_;
    }

    QList<Symbol> SharedEditor::text() const {
        return text_;
    }

    QString SharedEditor::to_string() const {
        QString result;
        for (const auto& s : text_)
            result.append(s.value());
        return result;
    }
}