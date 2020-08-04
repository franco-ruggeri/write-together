/*
 * Thread-safe wrapper of SharedEditor.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QMutex>
#include <editor/crdt/SharedEditor.h>

namespace editor {
    class SafeSharedEditor {
        SharedEditor editor_;
        mutable QMutex m_editor_;

    public:
        SafeSharedEditor(int site_id, int site_counter);
        SafeSharedEditor(SafeSharedEditor&& other);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        QList<Symbol> text() const;
    };
}
