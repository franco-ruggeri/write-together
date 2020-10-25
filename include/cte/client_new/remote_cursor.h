#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtGui/QTextCursor>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

namespace cte {
    class RemoteCursor : public QWidget {
        Q_OBJECT

        QPointer<QTextEdit> editor_;
        QSharedPointer<QLabel> vertical_label_, horizontal_label_;
        QTextCursor cursor_;

    public:
        RemoteCursor(QTextEdit *editor, const QString& username, const QColor& color);
        void move(int position);
    };
}
