#include <cte/client/login_form.h>
#include <ui_login_form.h>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>

namespace cte {
    LoginForm::LoginForm(QWidget *parent) : QWidget(parent), login_already_clicked_(false) {
        ui_ = QSharedPointer<Ui::LoginForm>::create();
        ui_->setupUi(this);
        ui_->username->addAction(QIcon(":images/forms/username.png"), QLineEdit::LeadingPosition);
        ui_->password->addAction(QIcon(":images/forms/password.png"), QLineEdit::LeadingPosition);
        connect(ui_->go_to_signup, &QPushButton::clicked, this, &LoginForm::signup_request);
    }

    void LoginForm::on_login_clicked() {
        QString username = ui_->username->text();
        QString password = ui_->password->text();
        if (username.isEmpty() || password.isEmpty())
            QMessageBox::warning(this, "Empty fields", "Please insert username and password.");
        else {
            if (!login_already_clicked_) {
                login_already_clicked_ = true;
                ui_->form->setEnabled(false);
                emit login_request(username, password);
            }
        }
    }

    void LoginForm::clear() {
        enable_form(true);
        ui_->username->clear();
        ui_->password->clear();
        ui_->username->setFocus();
    }

    void LoginForm::enable_form(bool enabled) {
        login_already_clicked_ = !enabled;
        ui_->form->setEnabled(enabled);
        if (enabled) ui_->username->setFocus();
    }
}