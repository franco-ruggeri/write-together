/*
 * Author: Franco Ruggeri
 */

#include <cte/server/OpenDocument.h>
#include <cte/crdt/Lseq.h>

namespace cte {
    OpenDocument::OpenDocument() :
            local_editor_(QSharedPointer<SharedEditor>::create(SharedEditor::invalid_site_id)),
            next_site_id_(SharedEditor::starting_site_counter), reference_count_(0) {}

    OpenDocument::OpenDocument(const QVector<character_t>& text) : OpenDocument() {
        QHash<QString,std::pair<int,int>> authors;
        QList<Symbol> symbols;
        Lseq pos_allocator;

        for (const auto& c : text) {
            if (!authors.contains(c.author)) {
                authors.insert(c.author, {next_site_id_, SharedEditor::starting_site_counter});
                site_ids_.insert(next_site_id_, c.author);
                next_site_id_++;
            }
            std::pair<int,int> author = authors[c.author];

            QVector<int> prev_pos = (c.index == 0 || symbols.empty()) ? pos_allocator.begin() : symbols.last().position();
            QVector<int> next_pos = pos_allocator.end();
            QVector<int> between_pos = pos_allocator.between(prev_pos, next_pos);
            
            symbols.push_back(Symbol(c.value, author.first, author.second++, between_pos));
        }

        local_editor_ = QSharedPointer<SharedEditor>::create(SharedEditor::invalid_site_id, symbols, pos_allocator);
    }

    int OpenDocument::open(const QString& username) {
        int site_id = next_site_id_;
        cursors_.insert(site_id, Symbol{});
        site_ids_.insert(site_id, username);
        next_site_id_++;
        reference_count_++;
        return site_id;
    }

    void OpenDocument::close(int site_id) {
        cursors_.remove(site_id);
        reference_count_--;
    }

    void OpenDocument::insert_symbol(const Symbol& symbol) {
        local_editor_->remote_insert(symbol);
    }

    void OpenDocument::erase_symbol(const Symbol& symbol) {
        local_editor_->remote_erase(symbol);
    }

    void OpenDocument::move_cursor(int site_id, const Symbol& symbol) {
        cursors_[site_id] = symbol;
    }

    QList<Symbol> OpenDocument::text() const {
        return local_editor_->text();
    }

    QHash<int,QString> OpenDocument::site_ids() const {
        return site_ids_;
    }

    QHash<int,Symbol> OpenDocument::cursors() const {
        return cursors_;
    }

    int OpenDocument::reference_count() const {
        return reference_count_;
    }
}
