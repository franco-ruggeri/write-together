#include <cte/client_new/profile_dialog.h>
#include <ui_profile_dialog.h>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include <QtGui/QPixmap>
#include <QtWidgets/QMessageBox>

namespace cte {
    ProfileDialog::ProfileDialog(const Profile& profile, bool editable, QWidget *parent) :
            profile_(profile), QDialog(parent) {
        ui_ = QSharedPointer<Ui::ProfileDialog>::create();
        ui_->setupUi(this);
        ui_->username->addAction(QIcon(":images/forms/username.png"), QLineEdit::LeadingPosition);
        ui_->email->addAction(QIcon(":images/forms/email.png"), QLineEdit::LeadingPosition);
        ui_->name->addAction(QIcon(":images/forms/name.png"), QLineEdit::LeadingPosition);
        ui_->surname->addAction(QIcon(":images/forms/surname.png"), QLineEdit::LeadingPosition);
        ui_->password->addAction(QIcon(":images/forms/password.png"), QLineEdit::LeadingPosition);
        ui_->repeat_password->addAction(QIcon(":images/forms/repeat_password.png"), QLineEdit::LeadingPosition);

        // fill UI with profile
        ui_->icon->setPixmap(QPixmap::fromImage(profile.icon()));
        ui_->username->setText(profile.username());
        ui_->email->setText(profile.email());
        ui_->name->setText(profile.name());
        ui_->surname->setText(profile.surname());

        // adapt for showcase
        if (!editable) {
            ui_->edit_icon->hide();
            ui_->email->setReadOnly(true);
            ui_->name->setReadOnly(true);
            ui_->surname->setReadOnly(true);
            ui_->email->setFocusPolicy(Qt::FocusPolicy::NoFocus);
            ui_->name->setFocusPolicy(Qt::FocusPolicy::NoFocus);
            ui_->surname->setFocusPolicy(Qt::FocusPolicy::NoFocus);
            ui_->password->hide();
            ui_->repeat_password->hide();
            ui_->cancel->hide();
            ui_->save->hide();
            adjustSize();
        }
    }

    void ProfileDialog::on_save_clicked() {
        profile_ = Profile(ui_->username->text(), ui_->name->text(), ui_->surname->text(), ui_->email->text(),
                           ui_->icon->pixmap()->toImage());
        QString password = ui_->password->text();

        if (!profile_.valid_email())
            QMessageBox::warning(this, "Invalid fields", "Please insert a valid email.");
        else if (!profile_.valid_name())
            QMessageBox::warning(this, "Invalid fields", "Please insert a name.");
        else if (!profile_.valid_surname())
            QMessageBox::warning(this, "Invalid fields", "Please insert a surname.");
        else if (password != ui_->repeat_password->text())
            QMessageBox::warning(this, "Invalid fields", "The passwords do not match.");
        else if (!password.isEmpty() && !Profile::valid_password(password))
            QMessageBox::warning(this, "Invalid fields", "The password must be at least 8 characters long and include "
                                                         "at least one number and both lower and upper characters.");
        else {
            if (password.isEmpty())
                    emit profile_update_request(profile_);
            else
                    emit profile_update_request(profile_, password);
        }
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