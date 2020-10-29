#include <cte/client/signup_form.h>
#include <cte/protocol/profile.h>
#include <cte/protocol/profile_message.h>
#include <ui_signup_form.h>
#include <QtGui/QImage>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>

namespace cte {
    SignupForm::SignupForm(QWidget *parent) : QWidget(parent) {
        ui_ = QSharedPointer<Ui::SignupForm>::create();
        ui_->setupUi(this);

        ui_->username->addAction(QIcon(":images/forms/username.png"), QLineEdit::LeadingPosition);
        ui_->email->addAction(QIcon(":images/forms/email.png"), QLineEdit::LeadingPosition);
        ui_->name->addAction(QIcon(":images/forms/name.png"), QLineEdit::LeadingPosition);
        ui_->surname->addAction(QIcon(":images/forms/surname.png"), QLineEdit::LeadingPosition);
        ui_->password->addAction(QIcon(":images/forms/password.png"), QLineEdit::LeadingPosition);
        ui_->repeat_password->addAction(QIcon(":images/forms/repeat_password.png"), QLineEdit::LeadingPosition);

        ui_->username->installEventFilter(this);
        ui_->email->installEventFilter(this);
        ui_->name->installEventFilter(this);
        ui_->surname->installEventFilter(this);
        ui_->password->installEventFilter(this);
        ui_->repeat_password->installEventFilter(this);

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
        else
            emit signup_request(profile, ui_->password->text());
    }

    bool SignupForm::eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
            if (key_event->key() == Qt::Key_Return)
                ui_->signup->click();
        }
        return false;
    }

    void SignupForm::clear() {
        ui_->username->clear();
        ui_->email->clear();
        ui_->name->clear();
        ui_->surname->clear();
        ui_->password->clear();
        ui_->repeat_password->clear();
        ui_->username->setFocus();
    }
}