#pragma once

#include <QtWidgets/QWidget>
#include <cte/widget/widget.h>

namespace cte {
    class Form : public Widget {
        Q_OBJECT

    public slots:
        virtual void clear() = 0;

    public:
        explicit Form(QWidget *parent=nullptr);
    };
}
