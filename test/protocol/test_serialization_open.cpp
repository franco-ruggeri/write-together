/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/OpenMessage.h>
#include <cte/protocol/Document.h>
#include <cte/protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <assert.h>

int main() {
    const cte::Document document("test owner", "test name");
    const QString sharing_link("fnsc:test_uri");
    const int site_id = 1;
    const cte::Profile profile("test username", "test name", "test surname", QImage{});

    QSharedPointer<cte::Message> message1, message2;

    // with document (owner + name), without optional arguments
    message1 = QSharedPointer<cte::OpenMessage>::create(document);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // with document (owner + name), with optional arguments
    message1 = QSharedPointer<cte::OpenMessage>::create(document, site_id, profile);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // with sharing link
    message1 = QSharedPointer<cte::OpenMessage>::create(sharing_link);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}