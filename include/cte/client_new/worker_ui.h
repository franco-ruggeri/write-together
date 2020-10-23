#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtWidgets/QStackedWidget>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>
#include <cte/client_new/connection_form.h>
#include <cte/client_new/login_form.h>
#include <cte/client_new/signup_form.h>
#include <cte/client_new/home.h>
#include <cte/client_new/editor.h>

namespace cte {
    class UiWorker : public QObject {
        Q_OBJECT

        QSharedPointer<QStackedWidget> forms_and_home_;
        QPointer<ConnectionForm> connect_form_;
        QPointer<LoginForm> login_form_;
        QPointer<SignupForm> signup_form_;
        QPointer<Home> home_;
        QHash<Document,QPointer<Editor>> editors_;

        void logged_in(const QSharedPointer<Message>& message);
        void show_document_list(const QSharedPointer<Message>& message);
        void open_editor(const QSharedPointer<Message>& message);
        void profile_updated(const QSharedPointer<Message>& message);
        void remote_insert(const QSharedPointer<Message>& message);
        void remote_erase(const QSharedPointer<Message>& message);
        void peer_open_document(const QSharedPointer<Message>& message);
        void peer_close_document(const QSharedPointer<Message>& message);

    signals:
        void connection_request(const QString& hostname, int port);
        void new_message(const QSharedPointer<Message>& message);

    private slots:
        void login(const QString& username, const QString& password);
        void signup(const Profile& profile, const QString& password);
        void logout();
        void create_document(const QString& document_name);
        void open_document(const Document& document);
        void open_document(const QUrl& sharing_link);
        void update_profile(const Profile& profile);
        void update_profile(const Profile& profile, const QString& password);
        void local_insert(const Document& document, const Symbol& symbol);
        void local_erase(const Document& document, const Symbol& symbol);
        void close_document(const Document& document);

    public slots:
        void show_connect_form();
        void show_login_form();
        void show_signup_form();
        void activate_home();
        void process_message(const QSharedPointer<Message>& message);

    public:
        explicit UiWorker(QObject *parent=nullptr);
        bool eventFilter(QObject *watched, QEvent *event) override;
    };
}
