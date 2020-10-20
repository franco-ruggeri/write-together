#include <cte/client_new/editor.h>
#include <cte/client_new/profile_dialog.h>
#include <ui_editor.h>
#include <QtCore/QStandardPaths>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QClipboard>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtPrintSupport/QPrinter>
#include <cmath>

// TODO: cursore visualizzato dopo il simbolo in cui si trova!!!! cosi' posso settarlo a shared_editor.begin() all'apertura
// TODO: occhio alle connect con le lambda, devo eliminare le connessioni senno' c'e' leakage
// TODO: find function
// TODO: disable MIME, HTML, markdown... only plain text (try in Qt designer first)
// TODO: document_id per efficienza
// TODO: merge signup_ok and profile_ok -> ok
// TODO: titolo finestre
// TODO: home come finestra a parte, form in stackedwidgets (?)
// TODO: per rimettere icona di default?
// TODO: aggiornamento profilo durante editing -> bisogna aggiornare il profilo in tutti i client, ci vuole un altro messaggio
// TODO: home chiusa -> non funziona pulsante in editor

namespace cte {
    Editor::Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent) :
            QMainWindow(parent), document_(document), sharing_link_(document_info.sharing_link()),
            shared_editor_(document_info.site_id(), document_info.text()), color_h_(std::floor(std::rand())),
            remote_changes_(0) {
        // create UI
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setText(shared_editor_.to_string());

        // create users
        for (auto& u : document_info.users()) {
            Profile& profile = u.first;
            QList<int>& site_ids = u.second;

            QSharedPointer<User> user = QSharedPointer<User>::create(profile, site_ids, color_h_);
            username_users_.insert(profile.username(), user);
            for (const auto& si : site_ids)
                site_id_users_.insert(si, user);
        }

        // create cursors
        QHash<int,Symbol> cursors = document_info.cursors();
        for (auto it=cursors.begin(); it != cursors.end(); it++) {
            int site_id = it.key();
            Symbol& symbol = it.value();
            site_id_users_[site_id]->add_cursor(site_id, symbol);
        }

        // connect signals and slots
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_change);
        connect(ui_->action_home, &QAction::triggered, this, &Editor::home_focus);
        connect(ui_->action_export_pdf, &QAction::triggered, this, &Editor::export_pdf);
        connect(ui_->action_invite, &QAction::triggered, this, &Editor::show_sharing_link);
        connect(ui_->action_close, &QAction::triggered, this, &Editor::closed);
        connect(ui_->action_undo, &QAction::triggered, ui_->editor, &QTextEdit::undo);
        connect(ui_->action_redo, &QAction::triggered, ui_->editor, &QTextEdit::redo);
        connect(ui_->action_cut, &QAction::triggered, ui_->editor, &QTextEdit::cut);
        connect(ui_->action_copy, &QAction::triggered, ui_->editor, &QTextEdit::copy);
        connect(ui_->action_paste, &QAction::triggered, ui_->editor, &QTextEdit::paste);
        connect(&shared_editor_, qOverload<int,QChar>(&SharedEditor::remote_insert),
                this, qOverload<int,QChar>(&Editor::remote_insert));
        connect(&shared_editor_, qOverload<int>(&SharedEditor::remote_erase),
                this, qOverload<int>(&Editor::remote_erase));

        // show user list
        ui_->users->topLevelItem(0)->setExpanded(true);
        ui_->users->topLevelItem(1)->setExpanded(true);
        refresh_users();
    }

    void Editor::refresh_users() {
        // clear
        QTreeWidgetItem *online_users = ui_->users->topLevelItem(0);
        QTreeWidgetItem *offline_users = ui_->users->topLevelItem(1);
        online_users->takeChildren();
        offline_users->takeChildren();

        // populate
        for (const auto& u : username_users_) {
            Profile profile = u->profile();
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, profile.username());
            child->setIcon(0, QIcon(QPixmap::fromImage(profile.icon())));
            if (u->online()) online_users->addChild(child);
            else offline_users->addChild(child);
        }
    }

    void Editor::remote_insert(int index, QChar value) {
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(index);
        cursor.insertText(value);
        remote_changes_++;
    }

    void Editor::remote_insert(const Symbol& symbol) {
        shared_editor_.remote_insert(symbol);
    }

    void Editor::remote_erase(int index) {
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(index);
        cursor.deleteChar();
        remote_changes_++;
    }

    void Editor::remote_erase(const Symbol& symbol) {
        shared_editor_.remote_erase(symbol);
    }

    void Editor::process_change(int position, int chars_removed, int chars_added) {
        // triggered by a remote change => skip
        if (remote_changes_ > 0) {
            remote_changes_--;
            return;
        }

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
        message_box.setText(sharing_link_.toString());
        message_box.addButton(tr("Close"), QMessageBox::NoRole);
        QPushButton *copy_button = message_box.addButton(tr("Copy"), QMessageBox::YesRole);
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
            user = QSharedPointer<User>::create(profile, QList<int>{site_id}, color_h_);
            username_users_.insert(username, user);
        } else {
            user = *it;
        }
        user->add_cursor(site_id, SharedEditor::bof);
        site_id_users_[site_id] = user;

        // refresh UI
        refresh_users();
    }

    void Editor::remove_online_user(int site_id) {
        // remove cursor
        QSharedPointer<User> user = site_id_users_.value(site_id);
        user->remove_cursor(site_id);

        // refresh UI
        refresh_users();
    }

    void Editor::on_users_itemClicked(QTreeWidgetItem *item, int column) {
        // TODO
//        if (item->parent() == nullptr) return;  // top-level item
//        QString username = item->text(0);
//
//        disconnect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_change);
//
//        int start_pos = -1;
//        QList<Symbol> text = shared_editor_.text();
//        for (int i=0; i<text.size(); i++) {
//            int site_id = text[i].site_id();
//
//
//            if (file.site_ids()[s[i].site_id()] == username && start_pos == -1) {
//                start_pos = i;
//            }
//            if (file.site_ids()[s[i].site_id()] != username && start_pos != -1) {
//
//                if(selected)
//                    format.setBackground(Qt::transparent);
//                else
//                    format.setBackground(color_);
//                format.setFontPointSize(14);
//                QTextCursor cursor(editor->document());
//                cursor.setPosition(start);
//                cursor.setPosition(end, QTextCursor::KeepAnchor);
//                cursor.setCharFormat(format);
//
//                user->draw_background_char(editor.data(), start_pos, i);
//                start_pos = -1;
//            }
//        }
//        if (start_pos != -1)
//            user->draw_background_char(editor.data(), start_pos, i);
//
//        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_change);
//        user->selected = !user->selected;
//        // editor->setCurrentCharFormat(user->format);
    }

    void Editor::on_users_itemDoubleClicked(QTreeWidgetItem *item, int column) {
        if (item->parent() == nullptr) return;  // top-level item
        QString username = item->text(0);
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
