/*
 * Author: Franco Ruggeri
 */

#include <cte/server/Server.h>
#include <cte/server/IdentityManager.h>
#include <cte/server/DocumentManager.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <algorithm>

namespace cte {
    IdentityManager identity_manager;
    DocumentManager document_manager;

    Server::Server(int port, int n_workers, int saving_interval_ms) {
        // try to save (better to crash at startup if there are problems)
        qDebug() << "trying to save";
        document_manager.save();

        // launch thread for saver
        saver_ = QSharedPointer<Saver>::create(saving_interval_ms);
        auto *thread = new QThread(this);
        thread->start();
        saver_->moveToThread(thread);

        // launch thread pool for workers
        for (int i=0; i < n_workers; i++) {
            auto *thread = new QThread(this);
            thread->start();

            workers_.push_back(QSharedPointer<Worker>::create());
            workers_[i]->moveToThread(thread);

            for (int j=0; j<i; j++)
                Worker::connect(*workers_[i], *workers_[j]);
        }

        // listen
        if (listen(QHostAddress::Any, port)) {
            qDebug() << "listening on port" << this->serverPort();
        } else {
            qDebug() << "listen() failed";
            QCoreApplication::exit(EXIT_FAILURE);
        }
    }

    void Server::incomingConnection(qintptr socket_fd) {
        // assign socket to worker with minimum number of open connections
        QSharedPointer<Worker>& w = *std::min_element(
                workers_.begin(), workers_.end(), [](const auto& w1, const auto& w2) {
                    return w1->number_of_connections() < w2->number_of_connections();
                });
        w->assign_connection(socket_fd);
    }
}
