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

    public slots:
        void save();

    public:
        Saver(int period);  // in milliseconds
    };
}
