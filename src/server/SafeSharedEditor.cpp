/*
 * Author: Franco Ruggeri
 */

#include "SafeSharedEditor.h"

SafeSharedEditor::SafeSharedEditor() {}

void SafeSharedEditor::remote_insert(const Symbol &symbol) {
    QMutexLocker ml(&m_editor_);
    editor_.remote_insert(symbol);
}

void SafeSharedEditor::remote_erase(const Symbol &symbol) {
    QMutexLocker ml(&m_editor_);
    editor_.remote_erase(symbol);
}
