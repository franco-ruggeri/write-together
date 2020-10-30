#include <cte/server/open_document.h>
#include <cte/crdt/lseq.h>

namespace cte {
    OpenDocument::OpenDocument() :
            shared_editor_(QSharedPointer<SharedEditor>::create()),
            next_site_id_(SharedEditor::starting_site_counter), reference_count_(0) {}

    OpenDocument::OpenDocument(const QVector<character_t>& text, const QList<QString>& usernames) : OpenDocument() {
        // assign site_ids
        QHash<QString,std::pair<int,int>> authors;  // username -> {site_id, site_counter}
        for (const auto& u : usernames) {
            authors.insert(u, {next_site_id_, SharedEditor::starting_site_counter});
            usernames_.insert(next_site_id_, u);
            next_site_id_++;
        }

        // fill shared editor
        int index = 0;
        for (const auto& c : text) {
            std::pair<int,int> author = authors[c.author];
            shared_editor_->insert(author.first, author.second++, index++, c.value);
            formats_.push_back(c.format);
        }
    }

    int OpenDocument::open(const QString& username) {
        int site_id = next_site_id_;
        cursors_.insert(site_id, shared_editor_->bof());
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
        cursors_[symbol.site_id()] = symbol;
    }

    void OpenDocument::erase_symbol(int site_id, const Symbol& symbol) {
        shared_editor_->remote_erase(symbol);
        cursors_[site_id] = symbol;
    }

    void OpenDocument::move_cursor(int site_id, const Symbol& symbol) {
        cursors_[site_id] = symbol;
    }

    QList<std::pair<Symbol,Format>> OpenDocument::text() const {
        QList<Symbol> text = shared_editor_->text();
        QList<std::pair<Symbol,Format>> text_with_format;
        for (int i=0; i<text.size(); i++)
            text_with_format.push_back({text[i], formats_[i]});
        return text_with_format;
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
