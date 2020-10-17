#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <cte/protocol/message.h>

namespace cte {
    class Widget : public QWidget {
        Q_OBJECT

    public slots:
        virtual void clear() = 0;

    public:
        explicit Widget(QWidget *parent=nullptr);
    };
}
