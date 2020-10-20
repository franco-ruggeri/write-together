#include <cte/client_new/user.h>
#include <QtCore/QObject>

namespace cte {
    User::User(const Profile &profile, const QList<int>& site_ids, double color_h) :
            profile_(profile), site_ids_(site_ids) {
        generate_color(color_h);
    }

    void User::generate_color(double color_h) {
        static double golden_ratio_conjugate = 0.618033988749895;
        color_h += golden_ratio_conjugate;
        color_h = color_h - static_cast<int>(color_h);
        color_.setHsvF(color_h, 0.5, 0.95);
    }

    void User::add_cursor(int site_id, const Symbol& symbol) {
        // TODO
        cursors_.insert(site_id, 0);
    }

    void User::remove_cursor(int site_id) {
        cursors_.remove(site_id);
    }

    void User::show_profile() {
        if (!profile_dialog_.isNull()) {    // already opened
            profile_dialog_->activateWindow();
        } else {
            profile_dialog_ = new ProfileDialog(profile_, false);
            QObject::connect(profile_dialog_, &ProfileDialog::finished, profile_dialog_, &ProfileDialog::deleteLater);
            profile_dialog_->open();
        }
    }

    Profile User::profile() const {
        return profile_;
    }

    bool User::online() const {
        return !cursors_.isEmpty();
    }

    bool User::selected() const {
        return selected();
    }

    void User::set_selected(bool selected) {
        selected_ = selected;
    }
}
