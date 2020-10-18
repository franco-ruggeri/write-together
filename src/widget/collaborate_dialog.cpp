#include <cte/widget/collaborate_dialog.h>
#include <ui_collaborate_dialog.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    CollaborateDialog::CollaborateDialog(QWidget *parent) : QDialog(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::CollaborateDialog>::create();
        ui_->setupUi(this);
    }

    void CollaborateDialog::on_open_clicked() {
        QString sharing_link = ui_->sharing_link->text();
        emit collaborate(sharing_link);
    }
}