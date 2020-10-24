#include <cte/client_new/worker_ui.h>
#include <cte/protocol/error_message.h>
#include <cte/protocol/signup_message.h>
#include <cte/protocol/signup_ok_message.h>
#include <cte/protocol/login_message.h>
#include <cte/protocol/logout_message.h>
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
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

namespace cte {
    UiWorker::UiWorker(QObject *parent) : QObject(parent) {
        qRegisterMetaType<QSharedPointer<Message>>("QSharedPointer<Message>");

        // create UI
        forms_and_home_ = QSharedPointer<QStackedWidget>::create();
        QStackedWidget *forms_and_home = forms_and_home_.data();
        connect_form_ = new ConnectionForm(forms_and_home);
        login_form_ = new LoginForm(forms_and_home);
        signup_form_ = new SignupForm(forms_and_home);
        home_ = new Home(forms_and_home);
        forms_and_home_->setWindowTitle("Real-Time Collaborative Text Editor");
        forms_and_home_->addWidget(connect_form_);
        forms_and_home_->addWidget(login_form_);
        forms_and_home_->addWidget(signup_form_);
        forms_and_home_->addWidget(home_);
        forms_and_home_->installEventFilter(this);

        // connect signals and slots
        connect(connect_form_, &ConnectionForm::connection_request, this, &UiWorker::connection_request);
        connect(login_form_, &LoginForm::login_request, this, &UiWorker::login);
        connect(login_form_, &LoginForm::signup_request, this, &UiWorker::show_signup_form);
        connect(signup_form_, &SignupForm::signup_request, this, &UiWorker::signup);
        connect(signup_form_, &SignupForm::login_request, this, &UiWorker::show_login_form);
        connect(home_, &Home::new_document_request, this, &UiWorker::create_document);
        connect(home_, qOverload<const Document&>(&Home::document_request),
                this, qOverload<const Document&>(&UiWorker::open_document));
        connect(home_, qOverload<const QString&>(&Home::document_request),
                this, qOverload<const QUrl&>(&UiWorker::open_document));
        connect(home_, qOverload<const Profile&>(&Home::profile_update_request),
                this, qOverload<const Profile&>(&UiWorker::update_profile));
        connect(home_, qOverload<const Profile&, const QString&>(&Home::profile_update_request),
                this, qOverload<const Profile&, const QString&>(&UiWorker::update_profile));
        connect(home_, &Home::logout_request, this, &UiWorker::logout);
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

    void UiWorker::activate_home() {
        forms_and_home_->activateWindow();
    }

    void UiWorker::process_message(const QSharedPointer<Message>& message) {
        switch (message->type()) {
            case MessageType::profile:
            case MessageType::signup_ok:
                logged_in(message);
                break;
            case MessageType::profile_ok:
                profile_updated(message);
                break;
            case MessageType::documents:
                show_document_list(message);
                break;
            case MessageType::document:
                open_editor(message);
                break;
            case MessageType::open:
                peer_open_document(message);
                break;
            case MessageType::close:
                peer_close_document(message);
                break;
            case MessageType::insert:
                remote_insert(message);
                break;
            case MessageType::erase:
                remote_erase(message);
                break;
            case MessageType::cursor:
//                peer_move_cursor();
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
        home_->set_profile(profile);
        QSharedPointer<Message> message = QSharedPointer<SignupMessage>::create(profile, password);
        emit new_message(message);
    }

    void UiWorker::logout() {
        // open editors => ask for confirmation
        if (!editors_.isEmpty()) {
            QMessageBox::StandardButton response = QMessageBox::question(
                    forms_and_home_.data(),
                    tr("Log out?"),
                    tr("There are still open editors. Logging out will close them. Are you sure?")
            );
            if (response == QMessageBox::No) return;
            close_editors();
        }

        // logout
        QSharedPointer<Message> message = QSharedPointer<LogoutMessage>::create();
        emit new_message(message);
        show_login_form();
    }

    void UiWorker::create_document(const QString& document_name) {
        QSharedPointer<Message> message = QSharedPointer<CreateMessage>::create(document_name);
        emit new_message(message);
    }

    void UiWorker::open_document(const Document& document) {
        // already opened => activate window
        auto it = editors_.find(document);
        if (it != editors_.end()) {
            it.value()->activateWindow();
            return;
        }

        // not opened => request
        QSharedPointer<Message> message = QSharedPointer<OpenMessage>::create(document);
        emit new_message(message);
    }

    void UiWorker::open_document(const QUrl& sharing_link) {
        // already opened => activate window
        for (const auto& e : editors_)
            if (sharing_link == e->sharing_link()) {
                e->activateWindow();
                return;
            }

        // not opened => request
        QSharedPointer<Message> message = QSharedPointer<OpenMessage>::create(sharing_link);
        emit new_message(message);
    }

    void UiWorker::update_profile(const Profile& profile) {
        QSharedPointer<Message> message = QSharedPointer<ProfileMessage>::create(profile);
        emit new_message(message);
    }

    void UiWorker::update_profile(const Profile& profile, const QString& password) {
        QSharedPointer<Message> message = QSharedPointer<ProfileMessage>::create(profile, password);
        emit new_message(message);
    }

    void UiWorker::local_insert(const Document& document, const Symbol& symbol) {
        QSharedPointer<Message> message = QSharedPointer<InsertMessage>::create(document, symbol);
        emit new_message(message);
    }

    void UiWorker::local_erase(const Document& document, const Symbol& symbol) {
        QSharedPointer<Message> message = QSharedPointer<EraseMessage>::create(document, symbol);
        emit new_message(message);
    }

    void UiWorker::close_document(const Document& document) {
        // delete editor
        auto it = editors_.find(document);
        QPointer<Editor> editor = *it;
        it.value()->deleteLater();
        editors_.erase(it);

        // activate home
        activate_home();

        // send message
        QSharedPointer<Message> message = QSharedPointer<CloseMessage>::create(document);
        emit new_message(message);
    }

    void UiWorker::logged_in(const QSharedPointer<Message>& message) {
        // prepare home
        if (message->type() == MessageType::profile)
            home_->set_profile(message.staticCast<ProfileMessage>()->profile());
        forms_and_home_->setCurrentWidget(home_);
        forms_and_home_->show();

        // request document list
        QSharedPointer<Message> request = QSharedPointer<DocumentsMessage>::create();
        emit new_message(request);
    }

    void UiWorker::show_document_list(const QSharedPointer<Message>& message) {
        QSharedPointer<DocumentsMessage> documents_message = message.staticCast<DocumentsMessage>();
        home_->set_documents(*documents_message->documents());
    }

    void UiWorker::open_editor(const QSharedPointer<Message>& message) {
        QSharedPointer<DocumentMessage> document_message = message.staticCast<DocumentMessage>();

        // update home
        Document document = document_message->document();
        home_->add_document(document);

        // create editor
        DocumentInfo document_info = document_message->document_info();
        QPointer<Editor> editor = new Editor(document, document_info);
        editor->setWindowTitle(document.full_name());
        editor->installEventFilter(this);
        editors_.insert(document_message->document(), editor);

        // connect signals and slots
        // TODO: problema... le connessioni poi devo toglierle, non posso usare lambda
        Editor *e = editor.data();
        connect(e, &Editor::local_insert, [this, document](const Symbol& symbol) { local_insert(document, symbol); });
        connect(e, &Editor::local_erase, [this, document](const Symbol& symbol) { local_erase(document, symbol); });
        connect(e, &Editor::home_focus, this, &UiWorker::activate_home);
        connect(e, &Editor::closed, [this, document]() { close_document(document); });

        // show editor
        editor->show();
        forms_and_home_->showMinimized();
    }

    void UiWorker::close_editors() {
        for (auto& e : editors_)
            e->deleteLater();
        editors_.clear();
    }

    void UiWorker::profile_updated(const QSharedPointer<Message>& message) {
        home_->profile_updated();
    }

    void UiWorker::remote_insert(const QSharedPointer<Message>& message) {
        QSharedPointer<InsertMessage> insert_message = message.staticCast<InsertMessage>();
        Document document = insert_message->document();
        Symbol symbol = insert_message->symbol();
        editors_[document]->remote_insert(symbol);
    }

    void UiWorker::remote_erase(const QSharedPointer<Message>& message) {
        QSharedPointer<EraseMessage> erase_message = message.staticCast<EraseMessage>();
        Document document = erase_message->document();
        Symbol symbol = erase_message->symbol();
        editors_[document]->remote_erase(symbol);
    }

    void UiWorker::peer_open_document(const QSharedPointer<Message>& message) {
        QSharedPointer<OpenMessage> open_message = message.staticCast<OpenMessage>();
        Document document = *open_message->document();
        int site_id = *open_message->site_id();
        Profile profile = *open_message->profile();
        editors_[document]->add_online_user(site_id, profile);
    }

    void UiWorker::peer_close_document(const QSharedPointer<Message>& message) {
        QSharedPointer<CloseMessage> close_message = message.staticCast<CloseMessage>();
        Document document = close_message->document();
        int site_id = *close_message->site_id();
        editors_[document]->remove_online_user(site_id);
    }

    bool UiWorker::eventFilter(QObject *watched, QEvent *event) {
        // closing with open editors => ask for confirmation
        if (watched == forms_and_home_ && event->type() == QEvent::Close && forms_and_home_->currentWidget() == home_ &&
            !editors_.isEmpty()) {

            QMessageBox::StandardButton response = QMessageBox::question(
                    forms_and_home_.data(),
                    tr("Close the application?"),
                    tr("There are still open editors. Closing the application will close them. Are you sure?")
            );

            if (response == QMessageBox::No) {
                event->ignore();    // filtering is not enough, it must be set to ignored
                return true;
            }
            close_editors();
        }

        return false;
    }
}
