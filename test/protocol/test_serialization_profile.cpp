/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/ProfileMessage.h>
#include <cte/protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QImage>

int main() {
    // icon
    QImage icon(24, 24, QImage::Format_RGB32);
    icon.fill(QColor(0, 0, 255));

    const cte::Profile profile("test username", "test name", "test surname", icon);
    const QString password("test password");

    QSharedPointer<cte::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<cte::ProfileMessage>::create(profile, password);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<cte::ProfileMessage>::create(profile);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}