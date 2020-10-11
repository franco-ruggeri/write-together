/*
 * Author: Franco Ruggeri
 */

#include <cte/protocol/document_message.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>
#include <cte/crdt/shared_editor.h>
#include <cte/crdt/symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <assert.h>

int main() {
    const cte::Document document("test owner", "test name");
    const QString text_string("test text");
    const QString sharing_link("cte:test_uri");
    const QString username1("test username 1"), username2("test username 2");

    // text
    cte::SharedEditor editor(0);
    for (int i=0; i<text_string.size(); i++)
        editor.local_insert(i, text_string[i]);
    QList<cte::Symbol> text = editor.text();

    // cursors
    const QHash<int,cte::Symbol> cursors{
            {1, text[0]},
            {2, text[1]},
            {3, text[2]}
    };

    // site_ids
    const QMultiHash<int,QString> site_ids{
            {1, username1},
            {2, username2},
            {3, username1}
    };

    // profiles
    const QHash<QString,cte::Profile> profiles{
            {username1, cte::Profile(username1, "test name 1", "test surname 1", QImage{})},
            {username2, cte::Profile(username2, "test name 2", "test surname 2", QImage{})}
    };

    // serialization
    cte::DocumentInfo document_data(text, editor.site_id(), cursors, site_ids, profiles, sharing_link);
    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::DocumentMessage>::create(document, document_data);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}