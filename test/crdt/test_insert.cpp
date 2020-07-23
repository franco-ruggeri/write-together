
#include <iostream>
#include "SharedEditor.h"
#include <QJsonDocument>

using namespace collaborative_text_editor;

int main() {

    SharedEditor editor1(1);
    SharedEditor editor2(2);

    // inserisco un testo qualunque (LSEQ = 1)
    std::string initial_text("test");
    for (int i=0; i<initial_text.size(); i++) {
        Symbol s = editor1.local_insert(i, initial_text[i]);
        QJsonObject o = s.json();
        std::cout << QJsonDocument(o).toJson(QJsonDocument::Compact).toStdString() << std::endl;
    }
    // inserisco un testo tra i caratteri precedenti, dopo il primo carattere inserito tutti a tutti gli altri
    //viene assegnato lo stesso indice
    for (int i=1; i<initial_text.size()+1; i++) {
        Symbol s = editor1.local_insert(i, initial_text[i-1]);
        QJsonObject o = s.json();
        std::cout << QJsonDocument(o).toJson(QJsonDocument::Compact).toStdString() << std::endl;
    }

    // se eseguo la riga di sotto il programma crasha perchè il carattere 1 e 2 hanno lo stesso indice
    // Symbol s = editor1.local_insert(2, 'b');

    return 0;
}