/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <protocol/Document.h>
#include "SafeSharedEditor.h"

class OpenDocument {
    Document document_;
    QSharedPointer<SafeSharedEditor> local_copy_;
    int reference_count_;

public:
    Document document() const;
    QSharedPointer<SafeSharedEditor> local_copy() const;
    int reference_count() const;
};
