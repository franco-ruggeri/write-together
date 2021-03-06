#include <cte/crdt/shared_editor.h>
#include <algorithm>

namespace cte {
    const int SharedEditor::invalid_site_id = -1;
    const int SharedEditor::invalid_site_counter = -1;
    const int SharedEditor::starting_site_id = 0;
    const int SharedEditor::starting_site_counter = 0;
    const int SharedEditor::reserved_site_id = -2;

    SharedEditor::SharedEditor() : SharedEditor(invalid_site_id) {}

    SharedEditor::SharedEditor(int site_id) :
            site_id_(site_id), site_counter_(starting_site_counter) {
        // important: not in initialization list, because we have to be sure that an Lseq object is created
        // see https://www.tutorialspoint.com/cplusplus/cpp_static_members.htm
        bof_ = Symbol(QChar(), reserved_site_id, starting_site_counter, {Lseq::begin});
        eof_ = Symbol(QChar(), reserved_site_id, starting_site_counter, {Lseq::end});
        text_.append(bof_);
        text_.append(eof_);
    }

    SharedEditor::SharedEditor(int site_id, const QList<Symbol>& text) :
            site_id_(site_id), site_counter_(starting_site_counter), text_(text) {
        // insert bof and eof (see note in the other constructor)
        bof_ = Symbol(QChar(), reserved_site_id, starting_site_counter, {Lseq::begin});
        eof_ = Symbol(QChar(), reserved_site_id, starting_site_counter, {Lseq::end});
        text_.insert(0, bof_);
        text_.append(eof_);

        // populate version vector
        for (const auto& s : text_)
            update_version_vector(s);
    }

    bool SharedEditor::existing_index(int index) const {
        return index >= 0 && index < text_.size()-2;
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
        if (index < 0 || index > text_.size()-2) throw std::logic_error("trying to insert at an invalid index");

        // allocate position
        index++;    // for BOF
        QVector<int> prev_pos = text_.at(index-1).position();
        QVector<int> next_pos = text_.at(index).position();
        QVector<int> between_pos = pos_allocator_.between(prev_pos, next_pos);

        // insert
        Symbol symbol(value, site_id, site_counter, between_pos);
        text_.insert(text_.begin()+index, symbol);
        update_version_vector(symbol);
        return symbol;
    }

    std::optional<int> SharedEditor::process_deletion_buffer() {
        QList<Symbol> deletion_buffer_copy(deletion_buffer_);
        deletion_buffer_.clear();

        std::optional<int> index;
        for (const auto& symbol : deletion_buffer_copy) {
            int site_id = symbol.site_id();
            int site_counter = symbol.site_counter();

            // check version vector
            auto it_site_counter = version_vector_.find(site_id);
            if (it_site_counter == version_vector_.end() || *it_site_counter < site_counter) {
                deletion_buffer_.append(symbol);
                continue;
            }

            // erase
            auto it_symbol = std::lower_bound(text_.begin(), text_.end(), symbol);
            if (it_symbol == text_.end() || !(*it_symbol == symbol)) continue;      // already erased
            index = std::distance(text_.begin(), it_symbol) - 1;    // -1 for BOF
            text_.erase(it_symbol);
        }
        return index;
    }

    Symbol SharedEditor::local_insert(int index, QChar value) {
        return insert(site_id_, site_counter_++, index, value);
    }

    Symbol SharedEditor::local_erase(int index) {
        if (!existing_index(index)) throw std::logic_error("trying to erase at an invalid index");
        index++;    // for BOF
        Symbol symbol = text_.at(index);
        text_.erase(text_.begin() + index);
        return symbol;
    }

    std::optional<int> SharedEditor::remote_insert(const Symbol& symbol) {
        if (symbol.value().isNull()) throw std::logic_error("trying to insert null character");

        // insert
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        int index = static_cast<int>(std::distance(text_.begin(), it) - 1); // -1 for BOF
        text_.insert(it, symbol);
        update_version_vector(symbol);

        // process deletion buffer
        bool erased = process_deletion_buffer().has_value();
        if (erased) return std::nullopt;    // inserted symbol was erased
        else return index;                  // return position
    }

    std::optional<int> SharedEditor::remote_erase(const Symbol& symbol) {
        if (symbol == bof_ || symbol == eof_) throw std::logic_error("trying to erase BOF or EOF");
        deletion_buffer_.append(symbol);
        return process_deletion_buffer();
    }

    std::optional<int> SharedEditor::find_symbol(const Symbol& symbol) const {
        if (symbol == bof_ || symbol == eof_) throw std::logic_error("trying to find BOF or EOF");
        auto it = std::lower_bound(text_.begin(), text_.end(), symbol);
        if (it == text_.end() || !(*it == symbol)) return std::nullopt;
        return static_cast<int>(std::distance(text_.begin(), it) - 1);    // -1 for BOF
    }

    Symbol SharedEditor::symbol_at(int index) const {
        if (!existing_index(index)) throw std::logic_error("trying to retrieve at an invalid index");
        index++;    // do not consider BOF
        return text_[index];
    }

    int SharedEditor::find_cursor(const Symbol& symbol) const {
        auto it = std::upper_bound(text_.begin(), text_.end(), symbol);
        return static_cast<int>(std::distance(text_.begin(), it) - 1);    // -1 to have <= (upper bound gives >)
    }

    Symbol SharedEditor::cursor_at(int index) const {
        return text_.at(index);
    }

    int SharedEditor::site_id() const {
        return site_id_;
    }

    QList<Symbol> SharedEditor::text() const {
        QList<Symbol> text(text_);
        text.removeFirst();     // BOF
        text.removeLast();      // EOF
        return text;
    }

    QString SharedEditor::to_string() const {
        QString result;
        for (auto it = text_.begin()+1; it != text_.end()-1; it++)  // skip BOF and EOF
            result.append(it->value());
        return result;
    }

    Symbol SharedEditor::bof() const {
        return bof_;
    }
}
