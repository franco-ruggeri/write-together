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
        explicit Saver(int period, QObject *parent=nullptr);  // period in milliseconds
    };
}
