#include <cte/client_new/user.h>
#include <QtCore/QObject>

namespace cte {
    User::User(const Profile &profile, const QSet<int>& site_ids, const QColor& color) :
            profile_(profile), site_ids_(site_ids), selected_(false), color_(color) {}

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

    QColor User::color() const {
        return color_;
    }

    bool User::selected() const {
        return selected_;
    }

    void User::toggle_selected() {
        selected_ = !selected_;
    }

    bool User::contains(int site_id) {
        return site_ids_.contains(site_id);
    }
}
