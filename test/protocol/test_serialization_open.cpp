#include <cte/protocol/open_message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>
#include <cassert>

int main() {
    const cte::Document document("test owner", "test name");
    const QUrl sharing_link("fnsc:test_uri");
    const int site_id = 1;
    const cte::Profile profile("test username", "test name", "test surname", "test email", QImage{});

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