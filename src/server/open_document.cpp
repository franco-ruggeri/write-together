/*
 * Author: Franco Ruggeri
 */

#include <cte/server/open_document.h>
#include <cte/crdt/lseq.h>

namespace cte {
    OpenDocument::OpenDocument() :
            shared_editor_(QSharedPointer<SharedEditor>::create(SharedEditor::invalid_site_id)),
            next_site_id_(SharedEditor::starting_site_counter), reference_count_(0) {}

    OpenDocument::OpenDocument(const QVector<character_t>& text) : OpenDocument() {
        QHash<QString,std::pair<int,int>> authors;
        int index = 0;

        for (const auto& c : text) {
            if (!authors.contains(c.author)) {
                authors.insert(c.author, {next_site_id_, SharedEditor::starting_site_counter});
                usernames_.insert(next_site_id_, c.author);
                next_site_id_++;
            }
            std::pair<int,int> author = authors[c.author];
            shared_editor_->insert(author.first, author.second++, index++, c.value);
        }
    }

    int OpenDocument::open(const QString& username) {
        int site_id = next_site_id_;
        cursors_.insert(site_id, SharedEditor::bof);
        usernames_.insert(site_id, username);
        next_site_id_++;
        reference_count_++;
        return site_id;
    }

    void OpenDocument::close(int site_id) {
        cursors_.remove(site_id);
        reference_count_--;
    }

    void OpenDocument::insert_symbol(const Symbol& symbol) {
        shared_editor_->remote_insert(symbol);
    }

    void OpenDocument::erase_symbol(const Symbol& symbol) {
        shared_editor_->remote_erase(symbol);
    }

    void OpenDocument::move_cursor(int site_id, const Symbol& symbol) {
        cursors_[site_id] = symbol;
    }

    QList<Symbol> OpenDocument::text() const {
        return shared_editor_->text();
    }

    QString OpenDocument::username(int site_id) const {
        return *usernames_.find(site_id);
    }

    QHash<int,Symbol> OpenDocument::cursors() const {
        return cursors_;
    }

    QHash<int,QString> OpenDocument::usernames() const {
        return usernames_;
    }

    int OpenDocument::reference_count() const {
        return reference_count_;
    }
}
