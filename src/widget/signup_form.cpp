#include <cte/widget/signup_form.h>
#include <cte/protocol/profile.h>
#include <cte/protocol/profile_message.h>
#include <ui_signup_form.h>
#include <QtGui/QImage>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    SignupForm::SignupForm(QWidget *parent) : QWidget(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::SignupForm>::create();
        ui_->setupUi(this);
        connect(ui_->go_to_login, &QPushButton::clicked, this, &SignupForm::login_request);
    }

    void SignupForm::on_signup_clicked() {
        // TODO: check password == confirm_password, check password ecc.
        Profile profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                        QImage(":/images/profile/default.png"));
        signup_request(profile, ui_->password->text());
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