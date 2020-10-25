#include <cte/client_new/remote_cursor.h>
#include <QtGui/QTextCursor>

#include <QDebug>

namespace cte {
    RemoteCursor::RemoteCursor(QTextEdit *editor, const QString& username, const QColor& color) :
            QWidget(editor), editor_(editor) {
//        cursor_.setPosition(10);     // TODO: set to 0
//        QRect rect = editor->cursorRect(cursor_);
//
//        qDebug() << rect.center().x() << rect.center().y();
//
//        // vertical label looks like the local cursor, but colored
//        QPixmap pixmap(2, rect.height());
//        pixmap.fill(color);
//        vertical_label_ = new QLabel(this);
//        vertical_label_->setPixmap(pixmap);
//        vertical_label_->move(rect.center());

//        // TODO: perche' -4 e perche' topRight().y() invece di .top()?
//        horizontal_label_ = new QLabel(this);
//        horizontal_label_->move(coordinates.center().x(), coordinates.topRight().y() - 4);
//        horizontal_label_->setAutoFillBackground(true);
//        horizontal_label_->setPalette(color);
//        horizontal_label_->setText(username);

        vertical_label_ = QSharedPointer<QLabel>::create(editor);
        cursor_ = editor->textCursor();
        cursor_.setPosition(10);
//        new_position = qMin( new_position, editor->toPlainText().size());
//        c->cursor_->setPosition(new_position);
//        c->current_position =  new_position;
        QPixmap pix(2, 24);
        pix.fill(color);
        vertical_label_->setPixmap(pix);
        const QRect qRect = editor->cursorRect(cursor_);
        vertical_label_->move(qRect.left(), qRect.top());
        vertical_label_->show();
        horizontal_label_ = QSharedPointer<QLabel>::create(editor);
        int x_pos = qRect.center().x();
        int y_pos = qRect.topRight().y() - 4;
        QFontMetrics fm(editor->font());
        QRect bounds = fm.boundingRect(username);
        horizontal_label_->move(x_pos,y_pos);
        horizontal_label_->setAutoFillBackground(true);
        horizontal_label_->setPalette(color);
        horizontal_label_->setText(username);
        horizontal_label_->show();
//        QTextCursor cursor(editor->document());
    }

    void RemoteCursor::move(int position) {
//        new_position = qMin(new_position, editor->toPlainText().size());
//        if (new_position != c->current_position) {
            cursor_.setPosition(position);
//            c->current_position = position;
            const QRect qRect = editor_->cursorRect(cursor_);
            vertical_label_->move(qRect.left(), qRect.top());
            vertical_label_->show();
            int x_pos = qRect.center().x();
            int y_pos = qRect.topRight().y() - 4;
            QFontMetrics fm(editor_->font());
            QRect bounds = fm.boundingRect(horizontal_label_->text());
            horizontal_label_->move(x_pos, y_pos);
            horizontal_label_->show();
//            format.setFontPointSize(14);
//        }
    }
}
