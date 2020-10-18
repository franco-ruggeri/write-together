#include <cte/widget/connect_form.h>
#include <ui_connect_form.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    ConnectForm::ConnectForm(QWidget *parent) : QWidget(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::ConnectForm>::create();
        ui_->setupUi(this);
    }

    void ConnectForm::on_connect_clicked() {
        emit connect_to_server(ui_->hostname->text(), ui_->port->value());
    }

    void ConnectForm::clear() {
        ui_->hostname->clear();
        ui_->port->clear();
    }
}