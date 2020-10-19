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

// TODO: find function
// TODO: disable MIME, HTML, markdown... only plain text (try in Qt designer first)
// TODO: document_id per efficienza
// TODO: merge signup_ok and profile_ok -> ok
// TODO: titolo finestre
// TODO: home come finestra a parte, form in stackedwidgets (?)
// TODO: per rimettere icona di default?
// TODO: aggiornamento profilo durante editing -> bisogna aggiornare il profilo in tutti i client, ci vuole un altro messaggio

namespace cte {
    Editor::Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent) :
            QMainWindow(parent), document_(document), sharing_link_(document_info.sharing_link()),
            usernames_(document_info.usernames()), shared_editor_(document_info.site_id(), document_info.text()),
            remote_changes_(0) {
        // create UI
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setText(shared_editor_.to_string());

        // get online users
        QHash<QString,Profile> profiles = document_info.profiles();
        QHash<int,Symbol> cursors = document_info.cursors();
        for (auto it=cursors.begin(); it != cursors.end(); it++) {
            QString username = usernames_[it.key()];
            online_users_.insert(username, profiles[username]);
            session_counts_[username]++;
        }

        // get offline users
        for (auto it=profiles.begin(); it != profiles.end(); it++) {
            QString username = it.key();
            if (!online_users_.contains(username))
                offline_users_.insert(username, profiles[username]);
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

    static void refresh_users_helper(QTreeWidgetItem *parent, QHash<QString,Profile> users) {
        // clear
        parent->takeChildren();

        // populate
        for (const Profile& p : users) {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, p.username());
            child->setIcon(0, QIcon(QPixmap::fromImage(p.icon())));
            parent->addChild(child);
        }
        parent->sortChildren(0, Qt::AscendingOrder);
    }

    void Editor::refresh_users() {
        refresh_users_helper(ui_->users->topLevelItem(0), online_users_);
        refresh_users_helper(ui_->users->topLevelItem(1), offline_users_);
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
        // TODO: add cursor
        QString username = profile.username();
        usernames_[site_id] = username;
        online_users_[username] = profile;
        offline_users_.remove(username);
        session_counts_[username]++;
        refresh_users();
    }

    void Editor::remove_online_user(int site_id) {
        // TODO: remove cursor
        QString username = usernames_[site_id];
        auto it_count = session_counts_.find(username);
        it_count.value()--;
        if (*it_count == 0) {
            session_counts_.erase(it_count);
            auto it_user = online_users_.find(username);
            offline_users_[username] = *it_user;
            online_users_.erase(it_user);
            refresh_users();
        }
    }

    void Editor::on_users_itemClicked(QTreeWidgetItem *item, int column) {
        // TODO
//        QString username = item->text().split(' ')[0];
//
//        auto user = username_to_user.find(username);
//        qDebug() << username;
//        qDebug() << user->color(); // ??
//        qDebug() << user->username();
//
//
//        int i = 0;
//        disconnect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
//        disconnect(editor.data(), &QTextEdit::textChanged, this, &texteditor::textChange);
//
//        int start_pos = -1;
//        auto s = shared_editor->text();
//
//        for (i = 0; i < s.size(); i++) {
//            if (file.site_ids()[s[i].site_id()] == username && start_pos == -1) {
//                start_pos = i;
//            }
//            if (file.site_ids()[s[i].site_id()] != username && start_pos != -1) {
//                user->draw_background_char(editor.data(), start_pos, i);
//                start_pos = -1;
//            }
//        }
//        if (start_pos != -1)
//            user->draw_background_char(editor.data(), start_pos, i);
//
//
//        connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
//        connect(editor.data(), &QTextEdit::textChanged, this, &texteditor::textChange);
//
//        user->selected = !user->selected;
//        // editor->setCurrentCharFormat(user->format);
    }

    void Editor::on_users_itemDoubleClicked(QTreeWidgetItem *item, int column) {
        if (item->parent() == nullptr) return;  // top-level item
        QString username = item->text(0);
        Profile profile = online_users_.contains(username) ?
                          online_users_.value(username) : offline_users_.value(username);
        ProfileDialog *profile_dialog = new ProfileDialog(profile, false);
        connect(profile_dialog, &ProfileDialog::close, profile_dialog, &ProfileDialog::deleteLater);
        profile_dialog->open();
    }

    void Editor::closeEvent(QCloseEvent *event) {
        emit closed();
        event->accept();
    }

    QUrl Editor::sharing_link() const {
        return sharing_link_;
    }
}
