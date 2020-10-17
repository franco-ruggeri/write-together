#include <cte/widget/signup_form.h>
#include <cte/protocol/profile.h>
#include <cte/protocol/profile_message.h>
#include <ui_signup_form.h>
#include <QtGui/QImage>

namespace cte {
    SignupForm::SignupForm(QWidget *parent) : Widget(parent) {
        ui_ = QSharedPointer<Ui::SignupForm>::create();
        ui_->setupUi(this);
    }

    void SignupForm::on_signup_clicked() {
        // TODO: check password == confirm_password, check password ecc.
        Profile profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                        QImage(":/images/profile/default.png"));
        signup(profile, ui_->password->text());
    }

    void SignupForm::clear() {
        ui_->username->clear();
        ui_->email->clear();
        ui_->name->clear();
        ui_->surname->clear();
        ui_->password->clear();
        ui_->password_confirm->clear();
    }
}