#include <cte/widget/editor.h>
#include <ui_editor.h>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

// TODO: find function
// TODO: disable MIME, HTML, markdown... only plain text (try in Qt designer first)
// TODO: document_info deve mandare tutti i profili che hanno accesso al documento, non solo chi ha inserito almeno un carattere
// TODO: document_id per efficienza
// TODO: merge signup_ok and profile_ok -> ok
// TODO: add ok as response to logout
// TODO: titolo finestre

namespace cte {
    Editor::Editor(int site_id, const QList<Symbol>& text, QWidget *parent) :
            QMainWindow(parent), shared_editor_(site_id, text), remote_changes_(0) {
        init_resource();
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setText(shared_editor_.to_string());

        // connect signals and slots
        connect(ui_->editor->document(), &QTextDocument::contentsChange, this, &Editor::process_change);
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
}