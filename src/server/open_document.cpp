#include <cte/server/open_document.h>
#include <cte/crdt/lseq.h>
#include <cte/database/database_guard.h>
#include <cte/database/database_utility.h>
#include <database_utility_secret.h>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace cte {
    OpenDocument::OpenDocument(Document document) :
            document_(std::move(document)), next_site_id_(SharedEditor::starting_site_counter), reference_count_(0) {
        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // load usernames and assign site_id
        QHash<QString,std::pair<int,int>> authors;  // username -> {site_id, site_counter}
        query = query_select_document_profiles(database, document_);
        execute_query(query);
        while (query.next()) {
            QString username = query.value("username").toString();
            authors.insert(username, {next_site_id_, SharedEditor::starting_site_counter});
            usernames_.insert(next_site_id_, username);
            next_site_id_++;
        }

        // load text
        query = query_select_document_text(database, document_);
        execute_query(query);
        int index = 0;
        while (query.next()) {
            std::pair<int,int>& author = authors[query.value("author").toString()];
            shared_editor_.insert(author.first, author.second++, index++, query.value("value").toString().at(0));
            formats_.push_back(Format(query.value("bold").toBool(), query.value("italic").toBool(),
                                      query.value("underlined").toBool()));
        }

        // commit transaction
        database.commit();
        qDebug() << "document loaded:" << document_.full_name();
    }

    OpenDocument::OpenDocument(const OpenDocument& other) {
        QMutexLocker ml(&other.mutex_);
        this->document_ = other.document_;
        this->shared_editor_ = other.shared_editor_;
        this->cursors_ = other.cursors_;
        this->usernames_ = other.usernames_;
        this->formats_ = other.formats_;
        this->next_site_id_ = other.next_site_id_;
        this->reference_count_ = other.reference_count_;
    }

    OpenDocument& OpenDocument::operator=(const OpenDocument& other) {
        if (this != &other) {
            QMutexLocker ml(&other.mutex_);
            this->document_ = other.document_;
            this->shared_editor_ = other.shared_editor_;
            this->cursors_ = other.cursors_;
            this->usernames_ = other.usernames_;
            this->formats_ = other.formats_;
            this->next_site_id_ = other.next_site_id_;
            this->reference_count_ = other.reference_count_;
        }
        return *this;
    }

    void OpenDocument::save() {
        QMutexLocker ml(&mutex_);

        // open connection and start transaction
        QSqlDatabase database = connect_to_database();
        DatabaseGuard dg(database);
        database.transaction();
        QSqlQuery query(database);

        // delete old document text
        query = query_delete_document_text(database, document_);
        execute_query(query);

        // insert updated document text
        QList<Symbol> text = shared_editor_.text();
        query = prepare_query_insert_character(database, document_);
        for (int i=0; i<text.size(); i++) {
            QString username = *usernames_.find(text[i].site_id());
            QChar value = text[i].value();
            Format& format = formats_[i];
            bind_query_insert_character(query, i, username, value, format);
            execute_query(query);
        }

        // commit transaction
        database.commit();
        qDebug() << "document saved:" << document_.full_name();
    }

    int OpenDocument::open(const QString& username) {
        QMutexLocker ml(&mutex_);
        int site_id = next_site_id_;
        cursors_.insert(site_id, shared_editor_.bof());
        usernames_.insert(site_id, username);
        next_site_id_++;
        reference_count_++;
        return site_id;
    }

    void OpenDocument::close(int site_id) {
        QMutexLocker ml(&mutex_);
        cursors_.remove(site_id);
        reference_count_--;
    }

    void OpenDocument::insert_symbol(const Symbol& symbol, const Format& format) {
        QMutexLocker ml(&mutex_);
        std::optional<int> index = shared_editor_.remote_insert(symbol);
        if (index) formats_.insert(*index, format);
        cursors_[symbol.site_id()] = symbol;
    }

    void OpenDocument::erase_symbol(int site_id, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        std::optional<int> index = shared_editor_.remote_erase(symbol);
        if (index) formats_.removeAt(*index);
        cursors_[site_id] = symbol;
    }

    void OpenDocument::move_cursor(int site_id, const Symbol& symbol) {
        QMutexLocker ml(&mutex_);
        cursors_[site_id] = symbol;
    }

    void OpenDocument::format_symbol(const Symbol& symbol, const Format& format) {
        QMutexLocker ml(&mutex_);
        std::optional<int> index = shared_editor_.find_symbol(symbol);
        if (index) formats_[*index] = format;
    }

    QList<std::pair<Symbol,Format>> OpenDocument::text() const {
        QMutexLocker ml(&mutex_);
        QList<Symbol> text = shared_editor_.text();
        QList<std::pair<Symbol,Format>> text_with_format;
        for (int i=0; i<text.size(); i++)
            text_with_format.push_back({text[i], formats_[i]});
        return text_with_format;
    }

    QHash<int,Symbol> OpenDocument::cursors() const {
        QMutexLocker ml(&mutex_);
        return cursors_;
    }

    QHash<int,QString> OpenDocument::usernames() const {
        QMutexLocker ml(&mutex_);
        return usernames_;
    }

    int OpenDocument::reference_count() const {
        QMutexLocker ml(&mutex_);
        return reference_count_;
    }
}
