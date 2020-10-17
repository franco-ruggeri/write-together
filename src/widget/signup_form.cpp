#include <cte/widget/signup_form.h>
#include <cte/protocol/profile.h>
#include <cte/protocol/profile_message.h>
#include <ui_signup_form.h>
#include <QtGui/QImage>

namespace cte {
    SignupForm::SignupForm(QWidget *parent) : Form(parent) {
        ui_ = QSharedPointer<Ui::SignupForm>::create();
        ui_->setupUi(this);
    }

    void SignupForm::on_signup_clicked() {
        Profile profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                        QImage(":/images/profile/default.png"));
        QSharedPointer<Message> message = QSharedPointer<ProfileMessage>::create(profile);
        emit new_message(message);
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