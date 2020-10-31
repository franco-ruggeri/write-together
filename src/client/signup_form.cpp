#include <cte/client/signup_form.h>
#include <cte/protocol/profile.h>
#include <ui_signup_form.h>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>

namespace cte {
    SignupForm::SignupForm(QWidget *parent) : QWidget(parent), signup_already_clicked_(false) {
        ui_ = QSharedPointer<Ui::SignupForm>::create();
        ui_->setupUi(this);

        ui_->username->addAction(QIcon(":images/forms/username.png"), QLineEdit::LeadingPosition);
        ui_->email->addAction(QIcon(":images/forms/email.png"), QLineEdit::LeadingPosition);
        ui_->name->addAction(QIcon(":images/forms/name.png"), QLineEdit::LeadingPosition);
        ui_->surname->addAction(QIcon(":images/forms/surname.png"), QLineEdit::LeadingPosition);
        ui_->password->addAction(QIcon(":images/forms/password.png"), QLineEdit::LeadingPosition);
        ui_->repeat_password->addAction(QIcon(":images/forms/repeat_password.png"), QLineEdit::LeadingPosition);

        connect(ui_->go_to_login, &QPushButton::clicked, this, &SignupForm::login_request);
    }

    void SignupForm::on_signup_clicked() {
        Profile profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                        QImage(":/images/profile/default.png"));
        QString password = ui_->password->text();

        if (!profile.valid_username())
            QMessageBox::warning(this, "Invalid fields", "The username must begin with an alphanumeric character and  "
                                                         "can contain alphanumeric characters, dashes or underscores.");
        else if (!profile.valid_email())
            QMessageBox::warning(this, "Invalid fields", "Please insert a valid email.");
        else if (!profile.valid_name())
            QMessageBox::warning(this, "Invalid fields", "Please insert a name.");
        else if (!profile.valid_surname())
            QMessageBox::warning(this, "Invalid fields", "Please insert a surname.");
        else if (password != ui_->repeat_password->text())
            QMessageBox::warning(this, "Invalid fields", "The passwords do not match.");
        else if (!Profile::valid_password(password))
            QMessageBox::warning(this, "Invalid fields", "The password must be at least 8 characters long and include "
                                                         "at least one number and both lower and upper characters.");
        else {
            if (!signup_already_clicked_) {
                signup_already_clicked_ = true;
                ui_->form->setEnabled(false);
                emit signup_request(profile, ui_->password->text());
            }
        }
    }

    void SignupForm::clear() {
        enable_form(true);
        ui_->username->clear();
        ui_->email->clear();
        ui_->name->clear();
        ui_->surname->clear();
        ui_->password->clear();
        ui_->repeat_password->clear();
        ui_->username->setFocus();
    }

    void SignupForm::enable_form(bool enabled) {
        signup_already_clicked_ = !enabled;
        ui_->form->setEnabled(enabled);
        if (enabled) ui_->username->setFocus();
    }
}