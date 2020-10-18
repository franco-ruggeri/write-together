#include <cte/widget/create_dialog.h>
#include <ui_create_dialog.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    CreateDialog::CreateDialog(QWidget *parent) : QDialog(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::CreateDialog>::create();
        ui_->setupUi(this);
    }

    void CreateDialog::on_create_clicked() {
        // TODO: check name (empty ecc.)
        QString document_name = ui_->document_name->text();
        emit create_document(document_name);
    }
}