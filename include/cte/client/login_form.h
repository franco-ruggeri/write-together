#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtWidgets/QWidget>

namespace Ui {
    class LoginForm;
}

namespace cte {
    class LoginForm : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::LoginForm> ui_;
        bool login_already_clicked_;

    signals:
        void login_request(const QString& username, const QString& password);
        void signup_request();

    private slots:
        void on_login_clicked();

    public slots:
        void clear();

    public:
        explicit LoginForm(QWidget *parent=nullptr);
        void enable_form(bool enabled);
    };
}
