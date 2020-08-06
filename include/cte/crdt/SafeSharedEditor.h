/*
 * Thread-safe wrapper of SharedEditor.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QMutex>
#include <cte/crdt/SharedEditor.h>

namespace cte {
    class SafeSharedEditor {
        SharedEditor editor_;
        mutable QMutex m_editor_;

    public:
        explicit SafeSharedEditor(int site_id);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        QList<Symbol> text() const;
    };
}
