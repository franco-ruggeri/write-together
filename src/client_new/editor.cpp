#include <cte/client_new/editor.h>
#include <cte/client_new/profile_dialog.h>
#include <ui_editor.h>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QStandardPaths>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QClipboard>
#include <QtGui/QTextCharFormat>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtPrintSupport/QPrinter>
#include <optional>
#include <cmath>

// TODO: cursore visualizzato dopo il simbolo in cui si trova!!!! cosi' posso settarlo a shared_editor.begin() all'apertura
// TODO: occhio alle connect con le lambda, devo eliminare le connessioni senno' c'e' leakage
// TODO: find function
// TODO: disable MIME, HTML, markdown... only plain text (try in Qt designer first)
// TODO: document_id per efficienza
// TODO: aggiornamento profilo durante editing -> bisogna aggiornare il profilo in tutti i client, ci vuole un altro messaggio
// TODO: login/signup/connection se messi a schermo intero fanno caga... dimensione fissa?
// TODO: CSS editor
// TODO: action_user_list
// TODO: puo' assegnare colore nero a utente, in tal caso rimpiazzalo (caso speciale), senno' non si vede niente
#include <QDebug>

namespace cte {
    Editor::Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent) :
            QMainWindow(parent), document_(document), sharing_link_(document_info.sharing_link()),
            shared_editor_(document_info.site_id(), document_info.text()), color_h_(std::floor(std::rand())) {
        // create UI
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setText(shared_editor_.to_string());
        setWindowTitle(document.full_name());

        // set cursor to beginning
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(0);
        ui_->editor->setTextCursor(cursor);

        // create users
        for (auto& u : document_info.users()) {
            Profile& profile = u.first;
            QList<int>& site_ids = u.second;

            QSharedPointer<User> user = QSharedPointer<User>::create(profile, site_ids.toSet(), generate_color());
            username_users_.insert(profile.username(), user);
            for (const auto& si : site_ids)
                site_id_users_.insert(si, user);
        }

        // add remote cursors
        int my_site_id = document_info.site_id();
        QHash<int,Symbol> cursors = document_info.cursors();
        for (auto it=cursors.begin(); it != cursors.end(); it++) {
            int site_id = it.key();
            Symbol& symbol = it.value();
            QSharedPointer<User> user = site_id_users_[site_id];
            if (site_id != my_site_id)
                user->add_remote_cursor(ui_->editor, site_id, symbol);
            else {
                user->set_local(true);
                local_user_ = user;
            }
        }

        // connect signals and slots
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
        connect(ui_->action_home, &QAction::triggered, this, &Editor::home_request);
        connect(ui_->action_export_pdf, &QAction::triggered, this, &Editor::export_pdf);
        connect(ui_->action_invite, &QAction::triggered, this, &Editor::show_sharing_link);
        connect(ui_->action_close, &QAction::triggered, this, &Editor::closed);
        connect(ui_->action_undo, &QAction::triggered, ui_->editor, &QTextEdit::undo);
        connect(ui_->action_redo, &QAction::triggered, ui_->editor, &QTextEdit::redo);
        connect(ui_->action_cut, &QAction::triggered, ui_->editor, &QTextEdit::cut);
        connect(ui_->action_copy, &QAction::triggered, ui_->editor, &QTextEdit::copy);
        connect(ui_->action_paste, &QAction::triggered, ui_->editor, &QTextEdit::paste);

        // show user list
        ui_->users->topLevelItem(0)->setExpanded(true);
        ui_->users->topLevelItem(1)->setExpanded(true);
        refresh_users();
    }

    QColor Editor::generate_color() {
        static double golden_ratio_conjugate = 0.618033988749895;
        color_h_ += golden_ratio_conjugate;
        color_h_ = color_h_ - static_cast<int>(color_h_);

        QColor color;
        color.setHsvF(color_h_, 0.5, 0.95);
        return color;
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
            QTreeWidgetItem *child = new QTreeWidgetItem;
            if (u->online()) online_users->addChild(child);
            else offline_users->addChild(child);

            // fill and color
            child->setText(0, profile.username() + (u->local() ? tr(" (you)") : ""));
            child->setIcon(0, QIcon(QPixmap::fromImage(profile.icon())));
            if (u->online() || u->selected())   // only if selected for offline users!
                child->setBackground(0, u->color());
        }

        // show counts
        online_users->setText(0, "online users (" + QString::number(online_users->childCount()) + ")");
        offline_users->setText(0, "offline users (" + QString::number(offline_users->childCount()) + ")");

        // sort
        online_users->sortChildren(0, Qt::AscendingOrder);
        offline_users->sortChildren(0, Qt::AscendingOrder);
    }

    void Editor::remote_insert(const Symbol& symbol) {
        std::optional<int> index = shared_editor_.remote_insert(symbol);
        if (index) {
            disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
            QTextCursor cursor = ui_->editor->textCursor();
            cursor.setPosition(*index);
            cursor.insertText(symbol.value());
            connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
        }
    }

    void Editor::remote_erase(const Symbol& symbol) {
        std::optional<int> index = shared_editor_.remote_erase(symbol);
        if (index) {
            disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
            QTextCursor cursor = ui_->editor->textCursor();
            cursor.setPosition(*index);
            cursor.deleteChar();
            connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
        }
    }

    void Editor::process_local_change(int position, int chars_removed, int chars_added) {
        // TODO: perche'?
        if (position ==  ui_->editor->textCursor().block().position() && chars_added > 1) {
            chars_added  = chars_added - chars_removed;
            chars_removed = 0;
        }

        // erase
        // TODO: perche' il controllo empty()? e' costoso shared_editor_.text()! O(N)
        for (int i=0; i<chars_removed && !shared_editor_.text().isEmpty(); i++) {
            Symbol symbol = shared_editor_.local_erase(position);
            emit local_erase(symbol);
        }

        // insert
        for (int i=0 ; i<chars_added; i++) {
            QChar value = ui_->editor->toPlainText()[position+i];
            Symbol symbol = shared_editor_.local_insert(position+i, value);
            emit local_insert(symbol);
        }
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

    void Editor::add_online_user(int site_id, const Profile& profile) {
        // add cursor
        QString username = profile.username();
        QSharedPointer<User> user;
        auto it = username_users_.find(username);
        if (it == username_users_.end()) {
            user = QSharedPointer<User>::create(profile, QSet<int>{site_id}, color_h_);
            username_users_.insert(username, user);
        } else {
            user = *it;
        }
        user->add_remote_cursor(ui_->editor, site_id, SharedEditor::bof);
        site_id_users_[site_id] = user;

        // refresh UI
        refresh_users();
    }

    void Editor::remove_online_user(int site_id) {
        // remove cursor
        QSharedPointer<User> user = site_id_users_.value(site_id);
        user->remove_remote_cursor(site_id);

        // refresh UI
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

        // update color text
        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
        QList<Symbol> text = shared_editor_.text();
        QTextCursor cursor(ui_->editor->textCursor());
        QTextCharFormat format;
        format.setBackground(user->selected() ? user->color() : Qt::transparent);
        cursor.setPosition(0);
        for (int i = 0; i < text.size(); i++) {
            if (user->contains(text[i].site_id())) {    // author
                cursor.setPosition(i, QTextCursor::KeepAnchor);
            } else {
                cursor.mergeCharFormat(format);
                cursor.setPosition(i, QTextCursor::MoveAnchor);
            }
        }
        cursor.mergeCharFormat(format);   // necessary to color also last characters
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_local_change);
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

    QUrl Editor::sharing_link() const {
        return sharing_link_;
    }
}
