#include <cte/protocol/close_message.h>
#include <cte/protocol/document.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <assert.h>

int main() {
    const cte::Document document("test owner", "test name");
    const int site_id = 1;

    QSharedPointer<cte::Message> message1, message2;

    // with optional argument
    message1 = QSharedPointer<cte::CloseMessage>::create(document, site_id);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    // without optional argument
    message1 = QSharedPointer<cte::CloseMessage>::create(document);
    message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}