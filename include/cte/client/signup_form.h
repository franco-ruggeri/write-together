#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtWidgets/QWidget>
#include <cte/protocol/profile.h>

namespace Ui {
    class SignupForm;
}

namespace cte {
    class SignupForm : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::SignupForm> ui_;
        bool signup_already_clicked_;

    signals:
        void signup_request(const Profile& profile, const QString& password);
        void login_request();

    private slots:
        void on_signup_clicked();

    public slots:
        void clear();

    public:
        explicit SignupForm(QWidget *parent=nullptr);
        void enable_form(bool enabled);
    };
}
