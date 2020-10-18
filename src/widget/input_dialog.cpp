#include <cte/widget/input_dialog.h>
#include <ui_input_dialog.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    InputDialog::InputDialog(const QString& prompt, QWidget *parent) : QDialog(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::InputDialog>::create();
        ui_->setupUi(this);
        ui_->prompt->setText(prompt);
    }

    void InputDialog::on_confirm_clicked() {
        QString input = ui_->input->text();
        emit confirm(input);
    }
}