#include <cte/client/connection_form.h>
#include <ui_connection_form.h>
#include <QtGui/QKeyEvent>

namespace cte {
    ConnectionForm::ConnectionForm(QWidget *parent) : QWidget(parent) {
        ui_ = QSharedPointer<Ui::ConnectionForm>::create();
        ui_->setupUi(this);
    }

    void ConnectionForm::on_connect_clicked() {
        emit connection_request(ui_->hostname->text(), ui_->port->value());
    }

    void ConnectionForm::clear() {
        ui_->hostname->clear();
        ui_->port->clear();
        ui_->hostname->setFocus();
    }
}