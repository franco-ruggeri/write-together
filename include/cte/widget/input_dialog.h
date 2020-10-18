#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

namespace Ui {
    class InputDialog;
}

namespace cte {
    class InputDialog : public QDialog {
        Q_OBJECT

        QSharedPointer<Ui::InputDialog> ui_;

    signals:
        void confirm(const QString& input);

    private slots:
        void on_confirm_clicked();

    public:
        explicit InputDialog(const QString& prompt, QWidget *parent=nullptr);
    };
}
