#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <cte/protocol/message.h>

namespace cte {
    class Widget : public QWidget {
        Q_OBJECT

    signals:
        void new_message(const QSharedPointer<Message>& message);

    public:
        explicit Widget(QWidget *parent=nullptr);
    };
}
