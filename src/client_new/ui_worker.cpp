#include <cte/client_new/ui_worker.h>
#include <cte/widget/connect_form.h>
#include <cte/widget/login_form.h>
#include <cte/widget/signup_form.h>

namespace cte {
    UiWorker::UiWorker(QObject *parent) : QObject(parent) {
        // create UI
        forms_and_home_ = QSharedPointer<QStackedWidget>::create();
        QStackedWidget *fh = forms_and_home_.data();
        connect_form_ = new ConnectForm(fh);
        login_form_ = new LoginForm(fh);
        signup_form_ = new SignupForm(fh);
        home_ = new Home(fh);
        forms_and_home_->addWidget(connect_form_);
        forms_and_home_->addWidget(login_form_);
        forms_and_home_->addWidget(signup_form_);
        forms_and_home_->addWidget(home_);

        // connect signals for forwarding
        ConnectForm *cf = qobject_cast<ConnectForm*>(connect_form_);
        connect(cf, &ConnectForm::connect_to_server, this, &UiWorker::connect_to_server);
        connect(login_form_, &Form::new_message, this, &UiWorker::new_message);
        connect(signup_form_, &Form::new_message, this, &UiWorker::new_message);
    }

    void UiWorker::show_login_form() {
        login_form_->clear();
        forms_and_home_->setCurrentWidget(login_form_);
        forms_and_home_->show();
    }

    void UiWorker::process_message(const QSharedPointer<Message>& message) {
        // TODO
    }
}
