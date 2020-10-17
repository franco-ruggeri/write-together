#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <cte/widget/form.h>
#include <cte/protocol/profile.h>

namespace Ui {
    class SignupForm;
}

namespace cte {
    class SignupForm : public Form {
        Q_OBJECT

        QSharedPointer<Ui::SignupForm> ui_;

    private slots:
        void on_signup_clicked();

    public slots:
        void clear() override;

    public:
        explicit SignupForm(QWidget *parent=nullptr);
    };
}
