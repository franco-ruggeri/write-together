/*
 * Author: Franco Ruggeri
 */

#include <editor/protocol/DocumentMessage.h>
#include <editor/protocol/Document.h>
#include <editor/protocol/DocumentData.h>
#include <editor/crdt/SharedEditor.h>
#include <editor/crdt/Symbol.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>

int main() {
    const cte::Document document("test owner", "test name");
    const QString text_string("test text");
    const int site_id(cte::SharedEditor::starting_site_id), site_counter(cte::SharedEditor::starting_site_counter);
    const QString sharing_link("fnsc:test_uri");
    const QString username1("test username 1"), username2("test username 2");

    // text
    cte::SharedEditor editor(site_id, site_counter);
    for (int i=0; i<text_string.size(); i++)
        editor.local_insert(i, text_string[i]);
    QList<cte::Symbol> text = editor.text();

    // site_id_others
    const QHash<QString,int> site_id_others{
            {username1, 1},
            {username2, 2}
    };

    // profile_others
    const QHash<QString,cte::Profile> profile_others{
            {username1, cte::Profile(username1, "test name 1", "test surname 1", QImage{})},
            {username2, cte::Profile(username2, "test name 2", "test surname 2", QImage{})}
    };

    // cursors
    const QHash<QString,cte::Symbol> cursors{
            {username1, text[0]},
            {username2, text[1]}
    };

    // serialization
    cte::DocumentData document_data(text, editor.site_id(), editor.site_counter(), site_id_others, profile_others,
                                    cursors, sharing_link);
    QSharedPointer<cte::Message> message1 = QSharedPointer<cte::DocumentMessage>::create(document, document_data);
    QSharedPointer<cte::Message> message2 = cte::Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}