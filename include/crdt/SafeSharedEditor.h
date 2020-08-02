/*
 * Thread-safe wrapper of SharedEditor.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QMutex>
#include <crdt/SharedEditor.h>

using namespace collaborative_text_editor;

class SafeSharedEditor {
    SharedEditor editor_;
    QMutex m_editor_;

public:
    SafeSharedEditor(int site_id);
    void remote_insert(const Symbol& symbol);
    void remote_erase(const Symbol& symbol);
};
