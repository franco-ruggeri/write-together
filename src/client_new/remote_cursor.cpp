#include <cte/client_new/remote_cursor.h>

namespace cte {
    RemoteCursor::RemoteCursor(QTextEdit *editor, const QString& username, const QColor& color) :
            QWidget(editor), cursor_(editor->textCursor()) {
        cursor_.setPosition(0);

        QPixmap pixmap(2, 24);  // TODO: la dimensione dovrebbe dipendere dal font?
        pixmap.fill(color);
        QRect coordinates = editor->cursorRect();

        vertical_label_ = new QLabel(this);
        vertical_label_->setPixmap(pixmap);
        vertical_label_->move(coordinates.left(), coordinates.top());

        // TODO: perche' -4 e perche' topRight().y() invece di .top()?
        horizontal_label_ = new QLabel(this);
        horizontal_label_->move(coordinates.center().x(), coordinates.topRight().y() - 4);
        horizontal_label_->setAutoFillBackground(true);
        horizontal_label_->setPalette(color);
        horizontal_label_->setText(username);
    }
}
