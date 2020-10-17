#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <cte/widget/form.h>

namespace Ui {
    class LoginForm;
}

namespace cte {
    class LoginForm : public Form {
        Q_OBJECT

        QSharedPointer<Ui::LoginForm> ui_;

    private slots:
        void on_login_clicked();

    public slots:
        void clear() override;

    public:
        explicit LoginForm(QWidget *parent=nullptr);
    };
}
