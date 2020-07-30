/*
 * Author: Franco Ruggeri
 */

#include <protocol/ProfileMessage.h>
#include <protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QImage>

using namespace collaborative_text_editor;

int main() {
    // icon
    QImage icon(24, 24, QImage::Format_RGB32);
    icon.fill(QColor(0, 0, 255));

    const Profile profile("test username", "test name", "test surname", icon);
    const QString password("test password");

    QSharedPointer<Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<ProfileMessage>::create(profile, password);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<ProfileMessage>::create(profile);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}