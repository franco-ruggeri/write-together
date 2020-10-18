#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

namespace Ui {
    class LoginForm;
}

namespace cte {
    class LoginForm : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::LoginForm> ui_;

    signals:
        void login(const QString& username, const QString& password);
        void go_to_signup();

    private slots:
        void on_login_clicked();

    public slots:
        void clear();

    public:
        explicit LoginForm(QWidget *parent=nullptr);
    };
}
