#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <cte/widget/widget.h>

namespace Ui {
    class Home;
}

namespace cte {
    class Home : public Widget {
        Q_OBJECT

        QSharedPointer<Ui::Home> ui_;

    public:
        explicit Home(QWidget *parent=nullptr);
    };
}