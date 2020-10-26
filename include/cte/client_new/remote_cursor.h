#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtGui/QTextCursor>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>

namespace cte {
    class RemoteCursor {
        QPointer<QPlainTextEdit> editor_;
        QSharedPointer<QLabel> vertical_label_, horizontal_label_;
        QTextCursor cursor_;

    public:
        RemoteCursor(QPlainTextEdit *editor, const QString& username, const QColor& color);
        void move(int position);
        void refresh();
    };
}
