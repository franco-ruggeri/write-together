/*
 * Author: Franco Ruggeri
 */

#include <cte/crdt/shared_editor.h>
#include <algorithm>

namespace cte {
    const int SharedEditor::invalid_site_id = -1;
    const int SharedEditor::invalid_site_counter = -1;
    const int SharedEditor::starting_site_id = 0;
    const int SharedEditor::starting_site_counter = 0;

    SharedEditor::SharedEditor(int site_id, QObject *parent) :
            QObject(parent), site_id_(site_id), site_counter_(starting_site_counter) {}

    SharedEditor::SharedEditor(int site_id, const QList<Symbol>& text, QObject *parent) :
            QObject(parent), site_id_(site_id), site_counter_(starting_site_counter), text_(text) {
        // populate version vector
        for (const auto& s : text_)
            update_version_vector(s);
    }

    void SharedEditor::update_version_vector(const Symbol& symbol) {
        int site_id = symbol.site_id();
        int site_counter = symbol.site_counter();

        auto it = version_vector_.find(site_id);
        if (it == version_vector_.end())
            version_vector_.insert(site_id, site_counter);
        else if (site_counter > *it)
            *it = site_counter;
    }

    Symbol SharedEditor::insert(int site_id, int site_counter, int index, QChar value) {
        if (value.isNull()) throw std::logic_error("trying to insert null character");

        // allocate position
        QVector<int> prev_pos = index == 0 ? Lseq::begin() : text_.at(index-1).position();
        QVector<int> next_pos = index == text_.size() ? Lseq::end() : text_.at(index).position();
        QVector<int> between_pos = pos_allocator_.between(prev_pos, next_pos);

        // insert
        Symbol symbol(value, site_id, site_counter, between_pos);
        text_.insert(text_.begin()+index, symbol);
        return symbol;
    }

    std::optional<int> SharedEditor::buffered_erase(const Symbol& symbol) {
        if (version_vector_[symbol.site_id()] < symbol.site_counter())
            return std::nullopt;

        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it == text_.end() || !(*it == symbol)) return std::nullopt;
        int pos = std::distance(text_.begin(), it);
        text_.erase(it);
        return pos;
    }

    Symbol SharedEditor::local_insert(int index, QChar value) {
        return insert(site_id_, site_counter_++, index, value);
    }

    Symbol SharedEditor::local_erase(int index) {
        Symbol symbol = text_.at(index);
        text_.erase(text_.begin() + index);
        return symbol;
    }

    void SharedEditor::remote_insert(const Symbol& symbol) {
        if (symbol.value().isNull()) throw std::logic_error("trying to insert null character");

        // insert
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        text_.insert(it, symbol);
        update_version_vector(symbol);

        // process deletion buffer
        bool erased = false;
        QList<Symbol> deletion_buffer_copy(deletion_buffer_);
        deletion_buffer_.clear();
        for (const auto& s : deletion_buffer_copy) {
            std::optional<int> index = buffered_erase(s);
            if (s == symbol) erased = true;     // inserted symbol has been erased
            else if (index) emit remote_erase(*index);
            else deletion_buffer_.append(s);
        }

        if (!erased)
            emit std::distance(text_.begin(), it);
    }

    void SharedEditor::remote_erase(const Symbol& symbol) {
        update_version_vector(symbol);
        QList<Symbol> deletion_buffer_copy(deletion_buffer_);
        deletion_buffer_copy.append(symbol);

        for (const auto& s : deletion_buffer_copy) {
            std::optional<int> index = buffered_erase(s);
            if (index) emit remote_erase(*index);
            else deletion_buffer_.append(s);
        }
    }

    int SharedEditor::find(const Symbol& symbol) const {
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        return std::distance(text_.begin(), it);
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
