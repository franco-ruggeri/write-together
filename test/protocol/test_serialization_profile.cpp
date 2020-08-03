/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/ProfileMessage.h>
#include <editor/protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QImage>

int main() {
    // icon
    QImage icon(24, 24, QImage::Format_RGB32);
    icon.fill(QColor(0, 0, 255));

    const editor::Profile profile("test username", "test name", "test surname", icon);
    const QString password("test password");

    QSharedPointer<editor::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<editor::ProfileMessage>::create(profile, password);
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<editor::ProfileMessage>::create(profile);
    message2 = editor::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}