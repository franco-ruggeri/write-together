/*
 * Client worker responsible for the UI. It manages all the windows, processes incoming messages and generates outgoing
 * ones.
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QMessageBox>
#include <cte/protocol/message.h>
#include <cte/protocol/document.h>
#include <cte/crdt/symbol.h>
#include <cte/client/connection_form.h>
#include <cte/client/login_form.h>
#include <cte/client/signup_form.h>
#include <cte/client/home.h>
#include <cte/client/editor.h>

namespace cte {
    class UiWorker : public QObject {
        Q_OBJECT

        QSharedPointer<QStackedWidget> forms_;
        QSharedPointer<QMessageBox> connection_loading_;
        QPointer<ConnectionForm> connection_form_;
        QPointer<LoginForm> login_form_;
        QPointer<SignupForm> signup_form_;
        QSharedPointer<Home> home_;
        QHash<Document,QPointer<Editor>> editors_;

        void show_initial_form();
        void show_error(const QSharedPointer<Message>& message);
        void logged_in(const QSharedPointer<Message>& message);
        void show_document_list(const QSharedPointer<Message>& message);
        void open_editor(const QSharedPointer<Message>& message);
        void profile_updated(const QSharedPointer<Message>& message);
        void remote_insert(const QSharedPointer<Message>& message);
        void remote_erase(const QSharedPointer<Message>& message);
        void remote_open_document(const QSharedPointer<Message>& message);
        void remote_close_document(const QSharedPointer<Message>& message);
        void remote_move_cursor(const QSharedPointer<Message>& message);
        void remote_format_symbol(const QSharedPointer<Message>& message);
        void close_editors();

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
        void local_insert(const Document& document, const Symbol& symbol, const Format& format);
        void local_erase(const Document& document, const Symbol& symbol);
        void local_cursor_move(const Document& document, int site_id, const Symbol& symbol);
        void local_format_change(const Document& document, const Symbol& symbol, const Format& format);
        void close_document(const Document& document);
        void activate_home();
        void show_signup_form();

    public slots:
        void show_connection_loading(const QString& hostname, int port);
        void show_connection_form();
        void show_login_form();
        void show_error(const QString& error);
        void process_message(const QSharedPointer<Message>& message);

    public:
        explicit UiWorker(QObject *parent=nullptr);
        bool eventFilter(QObject *watched, QEvent *event) override;
    };
}
