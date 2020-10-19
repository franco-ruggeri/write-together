#include <cte/widget/editor.h>
#include <ui_editor.h>
#include <QtCore/QStandardPaths>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QClipboard>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtPrintSupport/QPrinter>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

// TODO: find function
// TODO: disable MIME, HTML, markdown... only plain text (try in Qt designer first)
// TODO: document_info deve mandare tutti i profili che hanno accesso al documento, non solo chi ha inserito almeno un carattere
// TODO: document_id per efficienza
// TODO: merge signup_ok and profile_ok -> ok
// TODO: titolo finestre

namespace cte {
    Editor::Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent) :
            QMainWindow(parent), document_(document), sharing_link_(document_info.sharing_link()),
            shared_editor_(document_info.site_id(), document_info.text()), remote_changes_(0) {
        init_resource();
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setText(shared_editor_.to_string());

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

    void Editor::closeEvent(QCloseEvent *event) {
        emit closed();
        event->accept();
    }

    QUrl Editor::sharing_link() const {
        return sharing_link_;
    }
}
