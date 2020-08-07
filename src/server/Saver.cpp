/*
 * Author: Franco Ruggeri
 */

#include <Saver.h>
#include <DocumentManager.h>
#include <QtCore/QObject>

extern DocumentManager document_manager;

Saver::Saver(int interval_ms) {
    QObject::connect(&timer_, &QTimer::timeout, &document_manager, &DocumentManager::save);
    timer_.start(interval_ms);
}
