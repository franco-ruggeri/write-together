/*
 * Author: Franco Ruggeri
 */

#include <protocol/UserProfile.h>

namespace collaborative_text_editor {
    UserProfile::UserProfile() {}

    UserProfile::UserProfile(const QString& username) : username_(username) {}

    UserProfile::UserProfile(const QString &username, const QImage &icon) : username_(username), icon_(icon) {}

    QString UserProfile::username() const {
        return username_;
    }

    QImage UserProfile::icon() const {
        return icon_;
    }

    void UserProfile::clear() {
        username_.clear();
        icon_ = QImage{};
    }
}
