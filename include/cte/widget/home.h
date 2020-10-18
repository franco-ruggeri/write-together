#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidgetItem>
#include <cte/protocol/document.h>
#include <cte/protocol/profile.h>
#include <cte/protocol/document.h>

namespace Ui {
    class Home;
}

namespace cte {
    class Home : public QWidget {
        Q_OBJECT

        QSharedPointer<Ui::Home> ui_;

    signals:
        void create_document(const QString& document_name);
        void open_document(const Document& document);
        void collaborate(const QString& sharing_link);
        void logout();

    private slots:
        void on_new_document_clicked();
        void on_collaborate_clicked();
        void on_documents_cellClicked(int row, int column);

    public slots:
        void clear();

    public:
        explicit Home(QWidget *parent=nullptr);
        void set_profile(const Profile& profile);
        void set_documents(const QList<Document>& documents);
    };
}