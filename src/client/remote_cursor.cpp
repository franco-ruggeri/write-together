#include <cte/client/remote_cursor.h>

namespace cte {
    RemoteCursor::RemoteCursor(QPlainTextEdit *editor, const QString& username, const QColor& color) : editor_(editor) {
        // init cursor
        cursor_ = editor->textCursor();

        // vertical label looks like the local cursor, but colored
        vertical_label_ = QSharedPointer<QLabel>::create(editor);
        vertical_label_->setAutoFillBackground(true);
        vertical_label_->setPalette(color);
        vertical_label_->show();

        // horizontal label contains username
        horizontal_label_ = QSharedPointer<QLabel>::create(editor);
        horizontal_label_->setAutoFillBackground(true);
        horizontal_label_->setPalette(color);
        horizontal_label_->setText(username);
        horizontal_label_->setStyleSheet("font-size: 10pt;");
        horizontal_label_->show();

        // move at the beginning
        move(0);
    }

    void RemoteCursor::move(int position) {
        cursor_.setPosition(position);
        refresh();
    }

    void RemoteCursor::refresh() {
        // trim position (useful in case of local erase)
        int max_pos = editor_->toPlainText().size();
        cursor_.setPosition(qMin(cursor_.position(), max_pos));

        // refresh label positions
        QRect rect = editor_->cursorRect(cursor_);
        QFontMetrics font_metrics(editor_->font());
        vertical_label_->setFixedSize(rect.width(), rect.height());
        vertical_label_->move(rect.topLeft());
        horizontal_label_->move(rect.center().x(), rect.bottom()-font_metrics.height() -
            horizontal_label_->height()/3);
    }
}
