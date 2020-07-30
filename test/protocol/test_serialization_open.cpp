/*
 * Author: Franco Ruggeri
 */

#include <protocol/OpenMessage.h>
#include <protocol/Document.h>
#include <protocol/Profile.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

using namespace collaborative_text_editor;

int main() {
    const Document document("test owner", "test name");
    const QString sharing_link("fnsc:test_uri");
    const int site_id = 1;
    const Profile profile("test username", "test name", "test surname");

    QSharedPointer<Message> message1, message2;

    // with document (owner + name), without optional arguments
    message1 = QSharedPointer<OpenMessage>::create(document);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // with document (owner + name), with optional arguments
    message1 = QSharedPointer<OpenMessage>::create(document, site_id, profile);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // with sharing link
    message1 = QSharedPointer<OpenMessage>::create(sharing_link);
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}