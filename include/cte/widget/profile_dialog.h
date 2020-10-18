#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <cte/protocol/profile.h>

namespace Ui {
    class ProfileDialog;
}

namespace cte {
    class ProfileDialog : public QDialog {
        Q_OBJECT

        QSharedPointer<Ui::ProfileDialog> ui_;
        Profile profile_;

    signals:
        void update_profile(const Profile& profile);
        void update_profile(const Profile& profile, const QString& password);

    private slots:
        void on_update_clicked();
        void on_edit_icon_clicked();

    public:
        explicit ProfileDialog(const Profile& profile, QWidget *parent=nullptr);
        Profile profile() const;
    };
}
