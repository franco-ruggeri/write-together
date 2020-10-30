#include <cte/client/editor.h>
#include <cte/client/profile_dialog.h>
#include <ui_editor.h>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QClipboard>
#include <QtGui/QTextCharFormat>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtPrintSupport/QPrinter>
#include <optional>

namespace cte {
    Editor::Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent) :
            QMainWindow(parent), document_(document), sharing_link_(document_info.sharing_link()), copy_paste_(false) {
        // create UI
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setAcceptRichText(false);
        ui_->action_user_list->setChecked(true);
        setWindowTitle(document.full_name());

        // set text with format
        QList<std::pair<Symbol,Format>> text_with_format = document_info.text();
        QList<Symbol> text;
        local_cursor_ = ui_->editor->textCursor();
        for (int i=0; i<text_with_format.size(); i++) {
            auto& symbol_with_format = text_with_format[i];
            Symbol& s = symbol_with_format.first;
            Format& f = symbol_with_format.second;

            QTextCharFormat format;
            if (f.bold()) format.setFontWeight(1000);
            format.setFontItalic(f.italic());
            format.setFontUnderline(f.underlined());

            text.push_back(s);
            local_cursor_.setPosition(i);
            local_cursor_.mergeCharFormat(format);
            local_cursor_.insertText(s.value());
        }
        shared_editor_ = SharedEditor(document_info.site_id(), text);
        local_cursor_.setPosition(0);
        ui_->editor->setTextCursor(local_cursor_);

        // create users
        for (auto& u : document_info.users()) {
            Profile& profile = u.first;
            QList<int>& site_ids = u.second;

            QSharedPointer<User> user = QSharedPointer<User>::create(profile, site_ids);
            username_users_.insert(profile.username(), user);
            for (int si : site_ids)
                site_id_users_.insert(si, user);
        }

        // add remote cursors
        show();     // important: before adding remote cursors, to be initialized well!
        int my_site_id = document_info.site_id();
        QHash<int,Symbol> cursors = document_info.cursors();
        for (auto it=cursors.begin(); it != cursors.end(); it++) {
            int site_id = it.key();
            Symbol& symbol = it.value();
            QSharedPointer<User> user = site_id_users_[site_id];
            if (site_id != my_site_id) {
                int position = shared_editor_.find(symbol);
                user->add_remote_cursor(ui_->editor, site_id, position);
            } else {
                user->set_local(true);
                local_user_ = user;
                local_site_id_ = site_id;
            }
        }

        // show user list
        ui_->users->topLevelItem(0)->setExpanded(true);
        ui_->users->topLevelItem(1)->setExpanded(true);
        refresh_users();

        // show word and char count in status bar
        word_char_count_ = new QLabel;
        statusBar()->addWidget(word_char_count_);
        refresh_status_bar();

        // to handle bug (see comment in process_local_content_change())
        ui_->editor->installEventFilter(this);

        // connect signals and slots
        QTextDocument *editor_document = ui_->editor->document();
        connect(ui_->action_home, &QAction::triggered, this, &Editor::home_request);
        connect(ui_->action_export_pdf, &QAction::triggered, this, &Editor::export_pdf);
        connect(ui_->action_invite, &QAction::triggered, this, &Editor::show_sharing_link);
        connect(ui_->action_close, &QAction::triggered, this, &Editor::closed);
        connect(ui_->action_undo, &QAction::triggered, ui_->editor, &QTextEdit::undo);
        connect(ui_->action_redo, &QAction::triggered, ui_->editor, &QTextEdit::redo);
        connect(ui_->action_cut, &QAction::triggered, ui_->editor, &QTextEdit::cut);
        connect(ui_->action_copy, &QAction::triggered, ui_->editor, &QTextEdit::copy);
        connect(ui_->action_paste, &QAction::triggered, ui_->editor, &QTextEdit::paste);
        connect(ui_->dock, &QDockWidget::visibilityChanged, ui_->action_user_list, &QAction::setChecked);
        connect(ui_->action_user_list, &QAction::triggered, ui_->dock, &QDockWidget::setVisible);
        connect(ui_->editor, &QTextEdit::cursorPositionChanged, this, &Editor::process_local_cursor_move);
        connect(ui_->editor, &QTextEdit::textChanged, this, &Editor::refresh_status_bar);
        connect(ui_->editor->verticalScrollBar(), &QScrollBar::valueChanged, this, &Editor::refresh_cursors);
        connect(editor_document, &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
        connect(editor_document->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged,
                this, &Editor::refresh_cursors);
    }

    void Editor::refresh_cursors() {
        for (auto& u : username_users_)
            u->refresh_remote_cursors();
    }

    void Editor::refresh_status_bar() {
        int n_words = ui_->editor->toPlainText().split(R"((\s|\n|\r)+)", QString::SkipEmptyParts).count();
        int n_chars = ui_->editor->toPlainText().count();
        word_char_count_->setText("Words: " + QString::number(n_words) + "; Characters: " + QString::number(n_chars));
    }

    void Editor::refresh_users() {
        QTreeWidgetItem *online_users = ui_->users->topLevelItem(0);
        QTreeWidgetItem *offline_users = ui_->users->topLevelItem(1);

        // clear
        online_users->takeChildren();
        offline_users->takeChildren();

        // populate
        for (const auto& u : username_users_) {
            Profile profile = u->profile();
            auto *child = new QTreeWidgetItem;
            if (u->online()) online_users->addChild(child);
            else offline_users->addChild(child);

            // fill and color
            child->setText(0, profile.username() + (u->local() ? tr(" (you)") : ""));
            child->setIcon(0, QIcon(QPixmap::fromImage(profile.icon())));
            if (u->online() || u->selected())   // only if selected for offline users!
                child->setBackground(0, u->color());
        }

        // show counts
        online_users->setText(0, "Online users (" + QString::number(online_users->childCount()) + ")");
        offline_users->setText(0, "Offline users (" + QString::number(offline_users->childCount()) + ")");

        // sort
        online_users->sortChildren(0, Qt::AscendingOrder);
        offline_users->sortChildren(0, Qt::AscendingOrder);
    }

    void Editor::remote_insert(const Symbol& symbol) {
        // insert in shared editor
        std::optional<int> index = shared_editor_.remote_insert(symbol);
        if (!index) {       // erased using deletion buffer, no need to update UI editor
            remote_cursor_move(symbol.site_id(), symbol);
            return;
        }

        // insert in UI editor
        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(*index);
        cursor.insertText(symbol.value());
        qDebug() << "remote insert: { character:" << symbol.value() << ", position:" << *index << "}";

        // update remote cursor
        int site_id = symbol.site_id();
        site_id_users_[site_id]->move_remote_cursor(site_id, index.value()+1);  // set cursor after inserted char

        // update background color
        QSharedPointer<User> user = site_id_users_[site_id];
        QTextCharFormat format;
        format.setBackground(user->selected() ? user->color() : Qt::transparent);
        cursor.setPosition(index.value());
        cursor.setPosition(index.value()+1, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
    }

    void Editor::remote_erase(int site_id, const Symbol& symbol) {
        // erase from shared editor
        std::optional<int> index = shared_editor_.remote_erase(symbol);
        if (!index) {       // put in deletion buffer, do not update UI editor
            remote_cursor_move(site_id, symbol);
            return;
        }

        // erase from UI editor
        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(*index);
        cursor.deleteChar();
        qDebug() << "remote erase: { character:" << symbol.value() << ", position:" << *index << "}";

        // update remote cursor
        site_id_users_[site_id]->move_remote_cursor(site_id, *index);
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
    }

    void Editor::remote_cursor_move(int site_id, const Symbol& symbol) {
        disconnect(ui_->editor, &QTextEdit::cursorPositionChanged, this, &Editor::process_local_cursor_move);
        int index = shared_editor_.find(symbol);
        site_id_users_[site_id]->move_remote_cursor(site_id, index);
        connect(ui_->editor, &QTextEdit::cursorPositionChanged, this, &Editor::process_local_cursor_move);
        qDebug() << "remote cursor move: { site_id:" << site_id << ", position:" << index << "}";
    }

    void Editor::process_local_content_change(int position, int chars_removed, int chars_added) {
        qDebug() << "local change: { position:" << position
                 << ", chars_removed:" << chars_removed
                 << ", chars_added:" << chars_added << "}";

        // offset between symbols in shared_editor_ and characters in ui_->editor, see comment below about bug
        int offset = copy_paste_ ? 1 : 0;

        // signal erase
        for (int i=0; i<chars_removed; i++) {
            Symbol symbol = shared_editor_.local_erase(position - offset);
            emit local_erase(symbol);
            qDebug() << "local erase: { character:" << symbol.value() << ", position:" << position-offset << "}";
        }

        // signal insert
        for (int i=0 ; i<chars_added; i++) {
            // signal
            QChar value = ui_->editor->toPlainText()[position+i];
            Symbol symbol = shared_editor_.local_insert(position + i - offset, value);
            emit local_insert(symbol);
            qDebug() << "local insert: { character:" << value << ", position:" << position+i-offset << "}";
        }

        // update background color of inserted text
        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
        QTextCharFormat format;
        format.setBackground(local_user_->selected() ? local_user_->color() : Qt::transparent);
        local_cursor_.setPosition(position);
        local_cursor_.setPosition(position + chars_added, QTextCursor::KeepAnchor);
        local_cursor_.setCharFormat(format);
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);

        /*
         * There is a bug when copy-pasting at position 0, the arguments are completely wrong in such case.
         * As a workaround, we insert an empty character at position 0 in the event filter, so that the copy-paste
         * operation will never be at position 0. Here we remove this empty character.
         */
        if (copy_paste_) {
            disconnect(ui_->editor->document(), &QTextDocument::contentsChange,
                       this, &Editor::process_local_content_change);
            local_cursor_.setPosition(0);
            local_cursor_.deleteChar();
            copy_paste_ = false;
            connect(ui_->editor->document(), &QTextDocument::contentsChange,
                    this, &Editor::process_local_content_change);
        }

        // update cursors
        refresh_cursors();
        local_cursor_ = ui_->editor->textCursor();  // so that cursor move are not signalled for local insert/erase
    }

    void Editor::process_local_cursor_move() {
        int position = ui_->editor->textCursor().position();
        if (local_cursor_.position() == position) return;  // triggered by local insert/erase, do not signal anything
        local_cursor_.setPosition(position);
        Symbol symbol = shared_editor_.at(position);
        emit local_cursor_move(symbol);
        qDebug() << "local cursor move: { character:" << symbol.value() << ", position:" << position << "}";
    }

    void Editor::export_pdf() {
        QString directory = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).takeFirst() +
                QDir::separator() + document_.name() + ".pdf";
        QString filter = "PDF (*.pdf)";
        QString filename = QFileDialog::getSaveFileName(this, tr("Export as PDF"), directory, filter);
        if (filename.isNull()) return;
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        ui_->editor->print(&printer);
    }

    void Editor::show_sharing_link() {
        QMessageBox message_box(this);
        message_box.setWindowTitle(tr("Sharing link"));
        message_box.setText(sharing_link_.toString());
        message_box.addButton(tr("Close"), QMessageBox::NoRole);
        QPushButton *copy_button = message_box.addButton(tr("Copy"), QMessageBox::YesRole);
        message_box.setDefaultButton(copy_button);
        message_box.exec();
        if (message_box.clickedButton() == copy_button)
            QApplication::clipboard()->setText(message_box.text());
    }

    void Editor::remote_open(int site_id, const Profile& profile) {
        // add cursor
        QString username = profile.username();
        QSharedPointer<User> user;
        auto it = username_users_.find(username);
        if (it == username_users_.end()) {
            user = QSharedPointer<User>::create(profile, QList<int>{site_id});
            username_users_.insert(username, user);
        } else {
            user = *it;
        }
        user->add_remote_cursor(ui_->editor, site_id, 0);
        site_id_users_[site_id] = user;

        // refresh user list
        refresh_users();
    }

    void Editor::remote_close(int site_id) {
        // remove cursor
        QSharedPointer<User> user = site_id_users_.value(site_id);
        user->remove_remote_cursor(site_id);

        // refresh user list
        refresh_users();
    }

    void Editor::on_users_itemClicked(QTreeWidgetItem *item, int column) {
        if (item->parent() == nullptr) return;      // top-level item

        // toggle selection
        QString username = item->text(0).split(' ')[0];
        QSharedPointer<User> user = username_users_[username];
        user->toggle_selected();

        // offline user -> colored only if selected
        if (!user->online()) {
            if (user->selected()) item->setBackground(0, QBrush(user->color()));
            else item->setBackground(0, QBrush());
        }

        // prepare cursor with background color
        QTextCursor cursor(ui_->editor->textCursor());
        QTextCharFormat format;
        format.setBackground(user->selected() ? user->color() : Qt::transparent);
        cursor.setPosition(0);

        // update background color
        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
        QList<Symbol> text = shared_editor_.text();
        for (int i = 0; i < text.size(); i++) {
            int site_id = text[i].site_id();
            if (!user->contains(site_id)) {                         // not author
                cursor.setPosition(i, QTextCursor::KeepAnchor);     // color until here
                cursor.mergeCharFormat(format);
                cursor.setPosition(i+1, QTextCursor::MoveAnchor);
            }
        }
        cursor.setPosition(text.size(), QTextCursor::KeepAnchor);   // necessary to color also last characters
        cursor.mergeCharFormat(format);
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_content_change);
    }

    void Editor::on_users_itemDoubleClicked(QTreeWidgetItem *item, int column) {
        if (item->parent() == nullptr) return;  // top-level item
        QString username = item->text(0).split(" ")[0];
        username_users_[username]->show_profile();
    }

    void Editor::closeEvent(QCloseEvent *event) {
        emit closed();
        event->accept();
    }

    bool Editor::eventFilter(QObject *watched, QEvent *event) {
        // to handle bug (see comment in process_local_content_change())
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
            if (key_event->matches(QKeySequence::Paste)) {
                disconnect(ui_->editor->document(), &QTextDocument::contentsChange,
                           this, &Editor::process_local_content_change);
                local_cursor_.setPosition(0);
                local_cursor_.insertText(" ");
                copy_paste_ = true;
                connect(ui_->editor->document(), &QTextDocument::contentsChange,
                        this, &Editor::process_local_content_change);
            }
        }
        return false;
    }

    int Editor::local_site_id() const {
        return local_site_id_;
    }

    QUrl Editor::sharing_link() const {
        return sharing_link_;
    }
}
