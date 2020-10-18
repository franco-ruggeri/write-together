#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <cte/protocol/profile.h>

namespace Ui {
    class SignupForm;
}

namespace cte {
    class SignupForm : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::SignupForm> ui_;

    signals:
        void signup(const Profile& profile, const QString& password);

    private slots:
        void on_signup_clicked();

    public slots:
        void clear();

    public:
        explicit SignupForm(QWidget *parent=nullptr);
    };
}
