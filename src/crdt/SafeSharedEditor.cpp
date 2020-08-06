/*
 * Author: Franco Ruggeri
 */

#include <cte/crdt/SafeSharedEditor.h>

namespace cte {
    SafeSharedEditor::SafeSharedEditor(int site_id) : editor_(site_id) {}

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
