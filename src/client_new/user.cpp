#include <cte/client_new/user.h>
#include <QtCore/QObject>
#include <cmath>

namespace cte {
    User::User(const Profile &profile, const QList<int>& site_ids) :
            profile_(profile), selected_(false), online_(false), local_(false) {
        site_ids_ = QSet<int>::fromList(site_ids);
        generate_color();
    }

    void User::generate_color() {
        int site_id = *std::min_element(site_ids_.begin(), site_ids_.end());
        int hash = static_cast<int>(std::floor(site_id * 13)) % colors.size();
        color_ = colors[hash];
    }

    void User::add_remote_cursor(QPlainTextEdit *editor, int site_id) {
        QSharedPointer<RemoteCursor> cursor = QSharedPointer<RemoteCursor>::create(editor, profile_.username(), color_);
        remote_cursors_.insert(site_id, cursor);
        online_ = true;
    }

    void User::add_remote_cursor(QPlainTextEdit *editor, int site_id, int position) {
        add_remote_cursor(editor, site_id);
        move_remote_cursor(site_id, position);
    }

    void User::move_remote_cursor(int site_id, int position) {
        remote_cursors_.find(site_id).value()->move(position);
    }

    void User::remove_remote_cursor(int site_id) {
        remote_cursors_.remove(site_id);
        online_ = local_ || !remote_cursors_.isEmpty();
    }

    void User::refresh_remote_cursors() {
        for (auto& c : remote_cursors_)
            c->refresh();
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

    bool User::local() const {
        return local_;
    }

    void User::set_local(bool local) {
        local_ = local;
        online_ = online_ || local_;
    }

    Profile User::profile() const {
        return profile_;
    }

    bool User::online() const {
        return online_;
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

    // source: https://github.com/conclave-team/conclave/blob/master/lib/cssColors.js
    const QList<QColor> User::colors = {
            QColor(255, 0, 255),
            QColor(255, 51, 255),
            QColor(204, 0, 204),
            QColor(255, 102, 255),
            QColor(204, 51, 204),
            QColor(153, 0, 153),
            QColor(255, 153, 255),
            QColor(204, 102, 204),
            QColor(153, 51, 153),
            QColor(102, 0, 102),
            QColor(255, 204, 255),
            QColor(204, 153, 204),
            QColor(153, 102, 153),
            QColor(17, 117, 232),
            QColor(102, 51, 102),
            QColor(51, 0, 51),
            QColor(204, 0, 255),
            QColor(204, 51, 255),
            QColor(153, 0, 204),
            QColor(204, 102, 255),
            QColor(153, 51, 204),
            QColor(102, 0, 153),
            QColor(204, 153, 255),
            QColor(153, 102, 204),
            QColor(102, 51, 153),
            QColor(51, 0, 102),
            QColor(153, 0, 255),
            QColor(153, 51, 255),
            QColor(102, 0, 204),
            QColor(153, 102, 255),
            QColor(102, 51, 204),
            QColor(51, 0, 153),
            QColor(102, 0, 255),
            QColor(102, 51, 255),
            QColor(51, 0, 204),
            QColor(51, 0, 255),
            QColor(0, 0, 255),
            QColor(51, 51, 255),
            QColor(0, 0, 204),
            QColor(102, 102, 255),
            QColor(51, 51, 204),
            QColor(0, 0, 153),
            QColor(153, 153, 255),
            QColor(102, 102, 204),
            QColor(51, 51, 153),
            QColor(0, 0, 102),
            QColor(204, 204, 255),
            QColor(153, 153, 204),
            QColor(102, 102, 153),
            QColor(51, 51, 102),
            QColor(0, 0, 51),
            QColor(0, 51, 255),
            QColor(51, 102, 255),
            QColor(0, 51, 204),
            QColor(0, 102, 255),
            QColor(102, 153, 255),
            QColor(51, 102, 204),
            QColor(0, 51, 153),
            QColor(51, 153, 255),
            QColor(0, 102, 204),
            QColor(0, 153, 255),
            QColor(153, 204, 255),
            QColor(102, 153, 204),
            QColor(51, 102, 153),
            QColor(0, 51, 102),
            QColor(102, 204, 255),
            QColor(51, 153, 204),
            QColor(0, 102, 153),
            QColor(51, 204, 255),
            QColor(0, 153, 204),
            QColor(0, 204, 255),
            QColor(0, 255, 255),
            QColor(51, 255, 255),
            QColor(0, 204, 204),
            QColor(102, 255, 255),
            QColor(51, 204, 204),
            QColor(0, 153, 153),
            QColor(153, 255, 255),
            QColor(102, 204, 204),
            QColor(51, 153, 153),
            QColor(0, 102, 102),
            QColor(204, 255, 255),
            QColor(153, 204, 204),
            QColor(102, 153, 153),
            QColor(51, 102, 102),
            QColor(0, 51, 51),
            QColor(0, 255, 204),
            QColor(51, 255, 204),
            QColor(0, 204, 153),
            QColor(102, 255, 204),
            QColor(51, 204, 153),
            QColor(0, 153, 102),
            QColor(153, 255, 204),
            QColor(102, 204, 153),
            QColor(51, 153, 102),
            QColor(0, 102, 51),
            QColor(0, 255, 153),
            QColor(51, 255, 153),
            QColor(0, 204, 102),
            QColor(102, 255, 153),
            QColor(51, 204, 102),
            QColor(0, 153, 51),
            QColor(0, 255, 102),
            QColor(51, 255, 102),
            QColor(0, 204, 51),
            QColor(0, 255, 51),
            QColor(0, 255, 0),
            QColor(51, 255, 51),
            QColor(0, 204, 0),
            QColor(102, 255, 102),
            QColor(51, 204, 51),
            QColor(0, 153, 0),
            QColor(153, 255, 153),
            QColor(102, 204, 102),
            QColor(51, 153, 51),
            QColor(0, 102, 0),
            QColor(204, 255, 204),
            QColor(153, 204, 153),
            QColor(102, 153, 102),
            QColor(51, 102, 51),
            QColor(0, 51, 0),
            QColor(51, 255, 0),
            QColor(102, 255, 51),
            QColor(51, 204, 0),
            QColor(102, 255, 0),
            QColor(153, 255, 102),
            QColor(102, 204, 51),
            QColor(51, 153, 0),
            QColor(153, 255, 51),
            QColor(102, 204, 0),
            QColor(153, 255, 0),
            QColor(204, 255, 153),
            QColor(153, 204, 102),
            QColor(102, 153, 51),
            QColor(51, 102, 0),
            QColor(204, 255, 102),
            QColor(153, 204, 51),
            QColor(102, 153, 0),
            QColor(204, 255, 51),
            QColor(153, 204, 0),
            QColor(204, 255, 0),
            QColor(255, 255, 0),
            QColor(255, 255, 51),
            QColor(204, 204, 0),
            QColor(255, 255, 102),
            QColor(204, 204, 51),
            QColor(153, 153, 0),
            QColor(255, 255, 153),
            QColor(204, 204, 102),
            QColor(153, 153, 51),
            QColor(102, 102, 0),
            QColor(255, 255, 204),
            QColor(204, 204, 153),
            QColor(153, 153, 102),
            QColor(102, 102, 51),
            QColor(51, 51, 0),
            QColor(255, 204, 0),
            QColor(255, 204, 51),
            QColor(204, 153, 0),
            QColor(255, 204, 102),
            QColor(204, 153, 51),
            QColor(153, 102, 0),
            QColor(255, 204, 153),
            QColor(204, 153, 102),
            QColor(153, 102, 51),
            QColor(102, 51, 0),
            QColor(255, 153, 0),
            QColor(255, 153, 51),
            QColor(204, 102, 0),
            QColor(255, 153, 102),
            QColor(204, 102, 51),
            QColor(153, 51, 0),
            QColor(255, 102, 0),
            QColor(255, 102, 51),
            QColor(204, 51, 0),
            QColor(255, 51, 0),
            QColor(255, 0, 0),
            QColor(255, 51, 51),
            QColor(204, 0, 0),
            QColor(255, 102, 102),
            QColor(204, 51, 51),
            QColor(153, 0, 0),
            QColor(255, 153, 153),
            QColor(204, 102, 102),
            QColor(153, 51, 51),
            QColor(102, 0, 0),
            QColor(255, 204, 204),
            QColor(204, 153, 153),
            QColor(153, 102, 102),
            QColor(102, 51, 51),
            QColor(51, 0, 0),
            QColor(255, 0, 51),
            QColor(255, 51, 102),
            QColor(204, 0, 51),
            QColor(255, 0, 102),
            QColor(255, 102, 153),
            QColor(204, 51, 102),
            QColor(153, 0, 51),
            QColor(255, 51, 153),
            QColor(204, 0, 102),
            QColor(255, 0, 153),
            QColor(255, 153, 204),
            QColor(204, 102, 153),
            QColor(153, 51, 102),
            QColor(102, 0, 51),
            QColor(255, 102, 204),
            QColor(204, 51, 153),
            QColor(153, 0, 102),
            QColor(255, 51, 204)
    };
}
