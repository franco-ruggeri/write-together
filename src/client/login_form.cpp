#include <cte/client/login_form.h>
#include <cte/protocol/login_message.h>
#include <ui_login_form.h>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>

#include <QDebug>

namespace cte {
    LoginForm::LoginForm(QWidget *parent) : QWidget(parent), login_already_clicked_(false) {
        ui_ = QSharedPointer<Ui::LoginForm>::create();
        ui_->setupUi(this);
        ui_->username->addAction(QIcon(":images/forms/username.png"), QLineEdit::LeadingPosition);
        ui_->password->addAction(QIcon(":images/forms/password.png"), QLineEdit::LeadingPosition);
        ui_->username->installEventFilter(this);
        ui_->password->installEventFilter(this);
        connect(ui_->go_to_signup, &QPushButton::clicked, this, &LoginForm::signup_request);
    }

    void LoginForm::on_login_clicked() {
        QString username = ui_->username->text();
        QString password = ui_->password->text();
        if (username.isEmpty() || password.isEmpty())
            QMessageBox::warning(this, "Empty fields", "Please insert username and password.");
        else {
            qDebug() << "Click. Login: " << login_already_clicked_;
            if (!login_already_clicked_) {
                login_already_clicked_ = true;
                ui_->form->setEnabled(false);
                emit login_request(username, password);
            }
        }
    }

    bool LoginForm::eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
            if (key_event->key() == Qt::Key_Return)
                ui_->login->click();
        }
        return false;
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
    }
}