#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QTextCursor>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

namespace cte {
    class RemoteCursor : public QWidget {
        Q_OBJECT

        QTextCursor cursor_;
        QPointer<QLabel> vertical_label_, horizontal_label_;

    public:
        RemoteCursor(QTextEdit *editor, const QString& username, const QColor& color);
    };
}
