/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

namespace cte {
    class Saver : public QObject {
    Q_OBJECT

        QTimer timer_;

    public:
        Saver(int interval_ms);
    };
}
