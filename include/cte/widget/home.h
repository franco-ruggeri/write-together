#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
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
        void open_document(const Document& document_name);
        void logout();

    private slots:
        void on_new_document_clicked();

    public slots:
        void clear();

    public:
        explicit Home(QWidget *parent=nullptr);
        void set_profile(const Profile& profile);
        void set_documents(const QList<Document>& documents);
    };
}