#include <cte/client_new/ui_worker.h>
#include <cte/protocol/error_message.h>
#include <cte/protocol/signup_message.h>
#include <cte/protocol/signup_ok_message.h>
#include <cte/protocol/login_message.h>
#include <cte/protocol/profile_message.h>
#include <cte/protocol/profile_ok_message.h>
#include <cte/protocol/documents_message.h>
#include <cte/protocol/create_message.h>
#include <cte/protocol/open_message.h>
#include <cte/protocol/close_message.h>
#include <cte/protocol/document_message.h>
#include <cte/protocol/insert_message.h>
#include <cte/protocol/erase_message.h>
#include <cte/protocol/cursor_message.h>

namespace cte {
    UiWorker::UiWorker(QObject *parent) : QObject(parent) {
        qRegisterMetaType<QSharedPointer<Message>>("QSharedPointer<Message>");

        // create UI
        forms_and_home_ = QSharedPointer<QStackedWidget>::create();
        QStackedWidget *forms_and_home = forms_and_home_.data();
        connect_form_ = new ConnectForm(forms_and_home);
        login_form_ = new LoginForm(forms_and_home);
        signup_form_ = new SignupForm(forms_and_home);
        home_ = new Home(forms_and_home);
        forms_and_home_->addWidget(connect_form_);
        forms_and_home_->addWidget(login_form_);
        forms_and_home_->addWidget(signup_form_);
        forms_and_home_->addWidget(home_);

        // connect signals and slots
        connect(connect_form_, &ConnectForm::connect_to_server, this, &UiWorker::connect_to_server);
        connect(login_form_, &LoginForm::login, this, &UiWorker::login);
        connect(login_form_, &LoginForm::go_to_signup, this, &UiWorker::show_signup_form);
        connect(signup_form_, &SignupForm::signup, this, &UiWorker::signup);
        connect(home_, &Home::create_document, this, &UiWorker::create_document);
        connect(home_, &Home::open_document, this, &UiWorker::open_document);
        connect(home_, &Home::collaborate, this, &UiWorker::collaborate);
    }

    void UiWorker::show_connect_form() {
        connect_form_->clear();
        forms_and_home_->setCurrentWidget(connect_form_);
        forms_and_home_->show();
    }

    void UiWorker::show_login_form() {
        login_form_->clear();
        forms_and_home_->setCurrentWidget(login_form_);
        forms_and_home_->show();
    }

    void UiWorker::show_signup_form() {
        signup_form_->clear();
        forms_and_home_->setCurrentWidget(signup_form_);
        forms_and_home_->show();
    }

    void UiWorker::process_message(const QSharedPointer<Message>& message) {
        switch (message->type()) {
            case MessageType::profile:
            case MessageType::signup_ok:
                logged_in(message);
                break;
            case MessageType::documents:
                show_document_list(message);
                break;
            case MessageType::document:
                open_editor(message);
                break;
            case MessageType::open:
                break;
            case MessageType::close:
                break;
            case MessageType::insert:
                break;
            case MessageType::erase:
                break;
            case MessageType::cursor:
                break;
            default:    // should never happen, since the message is generated through Message::deserialize
                throw std::logic_error("invalid message: invalid type");
        }
    }

    void UiWorker::login(const QString& username, const QString& password) {
        QSharedPointer<Message> message = QSharedPointer<LoginMessage>::create(username, password);
        emit new_message(message);
    }

    void UiWorker::signup(const Profile& profile, const QString& password) {
        profile_ = profile;
        QSharedPointer<Message> message = QSharedPointer<SignupMessage>::create(profile, password);
        emit new_message(message);
    }

    void UiWorker::create_document(const QString& document_name) {
        QSharedPointer<Message> message = QSharedPointer<CreateMessage>::create(document_name);
        emit new_message(message);
    }

    void UiWorker::open_document(const Document& document) {
        QSharedPointer<Message> message = QSharedPointer<OpenMessage>::create(document);
        emit new_message(message);
    }

    void UiWorker::collaborate(const QString& sharing_link) {
        QSharedPointer<Message> message = QSharedPointer<OpenMessage>::create(sharing_link);
        emit new_message(message);
    }

    void UiWorker::logged_in(const QSharedPointer<Message>& message) {
        // save profile
        if (message->type() == MessageType::profile)
            profile_ = message.staticCast<ProfileMessage>()->profile();

        // prepare home
        home_->set_profile(profile_);
        forms_and_home_->setCurrentWidget(home_);
        forms_and_home_->show();

        // request document list
        QSharedPointer<Message> request = QSharedPointer<DocumentsMessage>::create();
        emit new_message(request);
    }

    void UiWorker::show_document_list(const QSharedPointer<Message>& message) {
        home_->set_documents(*message.staticCast<DocumentsMessage>()->documents());
    }

    void UiWorker::open_editor(const QSharedPointer<Message>& message) {
        QSharedPointer<DocumentMessage> document_message = message.staticCast<DocumentMessage>();

        // update home
        Document document = document_message->document();
        home_->add_document(document);

        // open editor
        DocumentInfo document_info = document_message->document_info();
        QSharedPointer<Editor> editor = QSharedPointer<Editor>::create(document_info.site_id(), document_info.text());
        editors_.insert(document_message->document(), editor);
        editor->show();
    }
}
