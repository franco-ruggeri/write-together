#include <cte/client_new/login_form.h>
#include <cte/protocol/login_message.h>
#include <ui_login_form.h>
#include <QtWidgets/QMessageBox>

namespace cte {
    LoginForm::LoginForm(QWidget *parent) : QWidget(parent) {
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
        else
            emit login_request(username, password);
    }

    void LoginForm::clear() {
        ui_->username->clear();
        ui_->password->clear();
    }
}