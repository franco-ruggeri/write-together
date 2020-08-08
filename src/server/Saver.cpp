/*
 * Author: Franco Ruggeri
 */

#include <cte/server/Saver.h>
#include <cte/server/DocumentManager.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

namespace cte {
    extern DocumentManager document_manager;

    Saver::Saver(int period) {
        // try to save (better to crash at startup if there are problems)
        save();

        // set timer
        connect(&timer_, &QTimer::timeout, this, &Saver::save);
        timer_.start(period);
    }

    void Saver::save() {
        try {
            document_manager.save();
        } catch (const std::exception &e) {
            // if it is not possible to save the open documents, the best choice is to crash...
            // otherwise the users would continue to edit thinking that everything is all right
            qDebug() << "fatal error -" << e.what();
            QCoreApplication::exit(EXIT_FAILURE);
        }
    }
}
