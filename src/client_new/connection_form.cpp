#include <cte/client_new/connection_form.h>
#include <ui_connection_form.h>
#include <QtGui/QKeyEvent>

namespace cte {
    ConnectionForm::ConnectionForm(QWidget *parent) : QWidget(parent) {
        ui_ = QSharedPointer<Ui::ConnectionForm>::create();
        ui_->setupUi(this);
        ui_->hostname->installEventFilter(this);
        ui_->port->installEventFilter(this);
    }

    void ConnectionForm::on_connect_clicked() {
        emit connection_request(ui_->hostname->text(), ui_->port->value());
    }

    bool ConnectionForm::eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
            if (key_event->key() == Qt::Key_Return)
                ui_->connect->click();
        }
        return false;
    }

    void ConnectionForm::clear() {
        ui_->hostname->clear();
        ui_->port->clear();
        ui_->hostname->setFocus();
    }
}