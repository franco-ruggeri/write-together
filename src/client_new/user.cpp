#include <cte/client_new/user.h>
#include <QtCore/QObject>
#include <QDebug>

namespace cte {
    User::User(const Profile &profile, const QSet<int>& site_ids, const QColor& color) :
            profile_(profile), site_ids_(site_ids), selected_(false), online_(false), local_(false), color_(color) {}

    void User::add_remote_cursor(QTextEdit *editor, int site_id, const Symbol& symbol) {
        QSharedPointer<RemoteCursor> cursor = QSharedPointer<RemoteCursor>::create(editor, profile_.username(), color_);
//        cursor->move()    // TODO
        cursor->show();
        remote_cursors_.insert(site_id, cursor);
        online_ = true;
        qDebug() << profile_.username() << online();
    }

    void User::remove_remote_cursor(int site_id) {
        remote_cursors_.remove(site_id);
        online_ = local_ || !remote_cursors_.isEmpty();
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
}
