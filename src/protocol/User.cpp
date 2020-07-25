/*
 * Author: Antonino Musmeci
 */

#include "protocol/User.h"

namespace collaborative_text_editor {
    User::User(const QString &username, const QImage &icon) : username_(username), icon_(icon) {}

    QImage User::icon() const {
        return icon_;
    }

    QString User::username() const {
        return username_;
    }

}