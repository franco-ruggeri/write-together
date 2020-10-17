#include <cte/widget/login_form.h>
#include <cte/protocol/login_message.h>
#include <ui_login_form.h>

namespace cte {
    LoginForm::LoginForm(QWidget *parent) : Widget(parent) {
        ui_ = QSharedPointer<Ui::LoginForm>::create();
        ui_->setupUi(this);

        connect(ui_->go_to_signup, &QPushButton::clicked, this, &LoginForm::go_to_signup);
    }

    void LoginForm::on_login_clicked() {
        emit login(ui_->username->text(), ui_->password->text());
    }

    void LoginForm::clear() {
        ui_->username->clear();
        ui_->password->clear();
    }
}