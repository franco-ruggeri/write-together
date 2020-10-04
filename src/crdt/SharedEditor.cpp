/*
 * Author: Franco Ruggeri
 */

#include <cte/crdt/SharedEditor.h>
#include <algorithm>

namespace cte {
    const int SharedEditor::invalid_site_id = -1;
    const int SharedEditor::invalid_site_counter = -1;
    const int SharedEditor::starting_site_id = 0;
    const int SharedEditor::starting_site_counter = 0;

    SharedEditor::SharedEditor(int site_id, const Lseq& pos_allocator) :
            site_id_(site_id), site_counter_(starting_site_counter), pos_allocator_(pos_allocator) {}

    SharedEditor::SharedEditor(int site_id, const QList<Symbol>& text, const Lseq& pos_allocator) :
            site_id_(site_id), site_counter_(starting_site_counter), text_(text), pos_allocator_(pos_allocator) {}

    Symbol SharedEditor::local_insert(int index, QChar value) {
       if (value.isNull()) throw std::logic_error("trying to insert null character");

        // allocate position
        QVector<int> prev_pos = index == 0 ? Lseq::begin() : text_.at(index-1).position();
        QVector<int> next_pos = index == text_.size() ? Lseq::end() : text_.at(index).position();
        QVector<int> between_pos = pos_allocator_.between(prev_pos, next_pos);

        // insert locally
        Symbol symbol(value, site_id_, site_counter_++, between_pos);
        text_.insert(text_.begin()+index, symbol);
        return symbol;
    }

    Symbol SharedEditor::insert_cursor(int index, QChar value) {
        /** TODO: ho aggiunto questo solo per provare i cursori, è da sistemare **/
        QVector<int> prev_pos = index == 0 ? Lseq::begin() : text_.at(index-1).position();
        QVector<int> next_pos = index == text_.size() ? Lseq::end() : text_.at(index).position();
        QVector<int> between_pos = pos_allocator_.between(prev_pos, next_pos);
        Symbol symbol(value, site_id_, site_counter_++, between_pos);
        return symbol;
    }

    Symbol SharedEditor::local_erase(int index) {
        Symbol symbol = text_.at(index);
        text_.erase(text_.begin() + index);
        return symbol;
    }

    void SharedEditor::remote_insert(const Symbol& symbol) {
        if (symbol.value().isNull()) throw std::logic_error("trying to insert null character");
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        text_.insert(it, symbol);
    }

    void SharedEditor::remote_erase(const Symbol& symbol) {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it != text_.end() && *it == symbol) text_.erase(it);
    }

    int SharedEditor::find(const Symbol& symbol) const {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it != text_.end()) return std::distance(text_.begin(), it);
        return text_.size();    // after last character
    }

    Symbol SharedEditor::at(int index) const {
        return text_.at(index);
    }

    int SharedEditor::site_id() const {
        return site_id_;
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
