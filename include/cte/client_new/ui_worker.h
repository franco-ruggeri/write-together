#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QStackedWidget>
#include <cte/protocol/message.h>
#include <cte/widget/form.h>
#include <cte/widget/home.h>
#include <cte/widget/editor.h>

namespace cte {
    class UiWorker : public QObject {
        Q_OBJECT

        QSharedPointer<QStackedWidget> forms_and_home_;
        QPointer<Form> connect_form_, login_form_, signup_form_;
        QPointer<Home> home_;

    signals:
        void connect_to_server(const QString& hostname, int port);
        void new_message(const QSharedPointer<Message>& message);

    public slots:
        void show_login_form();
        void process_message(const QSharedPointer<Message>& message);

    public:
        explicit UiWorker(QObject *parent=nullptr);
    };
}
