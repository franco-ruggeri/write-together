#include <cte/widget/profile_dialog.h>
#include <ui_profile_dialog.h>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include <QtGui/QPixmap>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    ProfileDialog::ProfileDialog(const Profile& profile, QWidget *parent) : profile_(profile), QDialog(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::ProfileDialog>::create();
        ui_->setupUi(this);
        ui_->icon->setPixmap(QPixmap::fromImage(profile.icon()));
        ui_->username->setText(profile.username());
        ui_->email->setText(profile.email());
        ui_->name->setText(profile.name());
        ui_->surname->setText(profile.surname());
    }

    void ProfileDialog::on_update_clicked() {
        // TODO: check password etc.
        profile_ = Profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                           ui_->icon->pixmap()->toImage());;
        QString password = ui_->password->text();
        if (password.isEmpty())
            emit update_profile(profile_);
        else
            emit update_profile(profile_, password);
    }

    void ProfileDialog::on_edit_icon_clicked() {
        QString directory = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).takeFirst();
        QString filter = tr("Images (*.png *.jpg *.jpeg)");
        QString filename = QFileDialog::getOpenFileName(this, tr("Select icon"), directory, filter);
        if (!filename.isNull()) ui_->icon->setPixmap(QPixmap(filename));
    }

    Profile ProfileDialog::profile() const {
        return profile_;
    }
}