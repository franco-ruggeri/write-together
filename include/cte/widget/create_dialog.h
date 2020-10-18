#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

namespace Ui {
    class CreateDialog;
}

namespace cte {
    class CreateDialog : public QDialog {
        Q_OBJECT

        QSharedPointer<Ui::CreateDialog> ui_;

    signals:
        void create_document(const QString& document_name);

    private slots:
        void on_create_clicked();

    public:
        explicit CreateDialog(QWidget *parent=nullptr);
    };
}
