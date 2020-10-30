#include <cte/protocol/document_message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>
#include <cte/protocol/format.h>
#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <cassert>

int main() {
    const cte::Document document("test owner", "test name");
    const QString text_string("test text");
    const QString sharing_link("cte:test_uri");
    const QString username1("test username 1"), username2("test username 2");

    // text
    cte::SharedEditor editor(0);
    QList<std::pair<cte::Symbol,cte::Format>> text = {
            {editor.local_insert(0, 't'), cte::Format(false, false, false)},
            {editor.local_insert(0, 'e'), cte::Format(true, false, false)},
            {editor.local_insert(0, 's'), cte::Format(false, true, false)},
            {editor.local_insert(0, 't'), cte::Format(false, false, true)},
            {editor.local_insert(0, 't'), cte::Format(true, true, false)},
            {editor.local_insert(0, 'e'), cte::Format(true, false, true)},
            {editor.local_insert(0, 'x'), cte::Format(false, true, true)},
            {editor.local_insert(0, 't'), cte::Format(true, true, true)}
    };

    // cursors
    const QHash<int,cte::Symbol> cursors{
            {1, text[0].first},
            {2, text[1].first},
            {3, text[2].first}
    };

    // users
    const QHash<QString,std::pair<cte::Profile,QList<int>>> users{
            {username1, {cte::Profile(username1, "test name 1", "test surname 1", "test email 1", QImage{}), {1, 3}}},
            {username2, {cte::Profile(username2, "test name 2", "test surname 2", "test email 2", QImage{}), {2}}}
    };

    // serialization
    cte::DocumentInfo document_data(text, editor.site_id(), cursors, users, sharing_link);
    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::DocumentMessage>::create(document, document_data);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}