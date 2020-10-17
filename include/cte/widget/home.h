#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <cte/protocol/document.h>
#include <cte/protocol/profile.h>
#include <cte/widget/widget.h>

namespace Ui {
    class Home;
}

namespace cte {
    class Home : public Widget {
        Q_OBJECT

        QSharedPointer<Ui::Home> ui_;

    public slots:
        void clear() override;

    public:
        explicit Home(QWidget *parent=nullptr);
        void set_profile(const Profile& profile);
        void set_documents(const QList<Document>& documents);
    };
}