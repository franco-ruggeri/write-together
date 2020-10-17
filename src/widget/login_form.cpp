#include <cte/widget/login_form.h>
#include <cte/protocol/login_message.h>
#include <ui_login_form.h>

namespace cte {
    LoginForm::LoginForm(QWidget *parent) : Form(parent) {
        ui_ = QSharedPointer<Ui::LoginForm>::create();
        ui_->setupUi(this);
    }

    void LoginForm::on_login_clicked() {
        QSharedPointer<Message> message = QSharedPointer<LoginMessage>::create(ui_->username->text(),
                                                                               ui_->password->text());
        emit new_message(message);
    }

    void LoginForm::clear() {
        ui_->username->clear();
        ui_->password->clear();
    }
}