#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

namespace Ui {
    class CollaborateDialog;
}

namespace cte {
    class CollaborateDialog : public QDialog {
        Q_OBJECT

        QSharedPointer<Ui::CollaborateDialog> ui_;

    signals:
        void collaborate(const QString& sharing_link);

    private slots:
        void on_open_clicked();

    public:
        explicit CollaborateDialog(QWidget *parent=nullptr);
    };
}
