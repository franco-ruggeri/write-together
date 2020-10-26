#pragma once

#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidgetItem>
#include <cte/client/profile_dialog.h>
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
        QSet<Document> documents_;
        Profile profile_;
        QPointer<ProfileDialog> profile_dialog_;

        void refresh_profile();
        void refresh_documents();

    signals:
        void new_document_request(const QString& document_name);
        void document_request(const Document& document);
        void document_request(const QString& sharing_link);
        void profile_update_request(const Profile& profile);
        void profile_update_request(const Profile& profile, const QString& password);
        void logout_request();

    private slots:
        void on_new_document_clicked();
        void on_collaborate_clicked();
        void on_profile_clicked();
        void on_documents_itemClicked(QListWidgetItem *item);

    public slots:
        void clear();

    public:
        explicit Home(QWidget *parent=nullptr);
        void set_profile(const Profile& profile);
        void set_documents(const QList<Document>& documents);
        void add_document(const Document& document);
        void profile_updated();
    };
}