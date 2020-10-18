#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QSet>
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

        enum class Filter {
            all_documents, your_documents, shared_with_you
        };

        QSharedPointer<Ui::Home> ui_;
        QSet<Document> documents_;
        Profile profile_;
        Filter filter_;

        void update_filter(Filter filter);
        void refresh();

    signals:
        void create_document(const QString& document_name);
        void open_document(const Document& document);
        void collaborate(const QString& sharing_link);
        void logout();

    private slots:
        void on_new_document_clicked();
        void on_collaborate_clicked();
        void on_all_documents_clicked();
        void on_your_documents_clicked();
        void on_shared_with_you_clicked();
        void on_documents_cellClicked(int row, int column);

    public slots:
        void clear();

    public:
        explicit Home(QWidget *parent=nullptr);
        void set_profile(const Profile& profile);
        void set_documents(const QList<Document>& documents);
        void add_document(const Document& document);
    };
}