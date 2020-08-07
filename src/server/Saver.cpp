/*
 * Author: Franco Ruggeri
 */

#include <cte/server/Saver.h>
#include <cte/server/DocumentManager.h>
#include <QtCore/QObject>

namespace cte {
    extern DocumentManager document_manager;

    Saver::Saver(int interval_ms) {
        QObject::connect(&timer_, &QTimer::timeout, &document_manager, &DocumentManager::save);
        timer_.start(interval_ms);
    }
}
