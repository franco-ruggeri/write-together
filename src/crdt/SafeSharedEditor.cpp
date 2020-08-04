/*
 * Author: Franco Ruggeri
 */

#include <editor/crdt/SafeSharedEditor.h>

namespace editor {
    SafeSharedEditor::SafeSharedEditor(int site_id, int site_counter) : editor_(site_id, site_counter) {}

    void SafeSharedEditor::remote_insert(const Symbol &symbol) {
        QMutexLocker ml(&m_editor_);
        editor_.remote_insert(symbol);
    }

    void SafeSharedEditor::remote_erase(const Symbol &symbol) {
        QMutexLocker ml(&m_editor_);
        editor_.remote_erase(symbol);
    }

    QList<Symbol> SafeSharedEditor::text() const {
        QMutexLocker ml(&m_editor_);
        return editor_.text();
    }
}
