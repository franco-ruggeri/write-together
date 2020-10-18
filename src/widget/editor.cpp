#include <cte/widget/editor.h>
#include <ui_editor.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    Editor::Editor(int site_id, const QList<Symbol>& text, QWidget *parent) :
            QMainWindow(parent), shared_editor_(site_id, text) {
        init_resource();
        ui_ = QSharedPointer<Ui::Editor>::create();
        ui_->setupUi(this);
        ui_->editor->setAcceptRichText(false);
        ui_->editor->setText(shared_editor_.to_string());
    }

    void Editor::remote_insert(const Symbol& symbol) {
        int pos = shared_editor_.remote_insert(symbol);
        QTextCursor cursor = ui_->editor->textCursor();
        cursor.setPosition(pos);
        cursor.insertText(symbol.value());
    }

    void Editor::remote_erase(const Symbol& symbol) {
        std::optional<int> pos = shared_editor_.remote_erase(symbol);
        if (pos) {
            QTextCursor cursor = ui_->editor->textCursor();
            cursor.setPosition(*pos);
            cursor.deleteChar();
        }
    }
}