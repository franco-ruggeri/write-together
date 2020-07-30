/*
 * Author: Franco Ruggeri
 */

#include <protocol/DocumentMessage.h>
#include <protocol/Document.h>
#include <crdt/SharedEditor.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <iostream>

using namespace collaborative_text_editor;

int main(int argc, char **argv) {
    QSharedPointer<Message> message1, message2;

    if (argc < 9) {
        std::cerr << "usage: " << argv[0]
                  << " document_owner document_name text sharing_link site_id username name surname" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // text
    SharedEditor editor(0);
    for (int i=0; argv[3][i]!='\0'; i++)
        editor.local_insert(i, argv[3][i]);
    QVector<Symbol> text = editor.text();

    // users
    QHash<int,Profile> users;
    Profile profile(argv[6], argv[7], argv[8]);
    users.insert(std::stoi(argv[5]), profile);

    // original message
    Document document(argv[1], argv[2]);
    QString sharing_link = argv[4];
    message1 = QSharedPointer<DocumentMessage>::create(document, text, users, sharing_link);

    // serialize -> deserialize
    message2 = Message::deserialize(message1->serialize());
    assert(*message1 == *message2);

    return 0;
}