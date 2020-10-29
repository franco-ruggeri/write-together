#include <cte/server/server.h>
#include <cte/server/identity_manager.h>
#include <cte/server/document_manager.h>
#include <QtCore/QPointer>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include <algorithm>

namespace cte {
    IdentityManager identity_manager;
    DocumentManager document_manager;

    Server::Server(int port, int n_workers, int saving_period) {
        QPointer<QThread> thread;

        // load certificate and private key
        QFile key_file(":/private.key");
        key_file.open(QIODevice::ReadOnly);
        private_key_ = QSslKey(key_file.readAll(), QSsl::Rsa);
        key_file.close();
        QFile cert_file(":/local_certificate.pem");
        cert_file.open(QIODevice::ReadOnly);
        local_certificate_ = QSslCertificate(cert_file.readAll());
        cert_file.close();

        // try to save (better to crash at startup if there are problems)
        qDebug() << "trying to save";
        document_manager.save();

        // launch thread for saver
        thread = new QThread(this);
        thread->start();
        saver_ = QSharedPointer<Saver>::create(saving_period);
        saver_->moveToThread(thread);

        // launch thread pool for workers
        for (int i=0; i < n_workers; i++) {
            thread = new QThread(this);
            thread->start();
            workers_.push_back(QSharedPointer<Worker>::create(this));
            workers_[i]->moveToThread(thread);
            for (int j=0; j<i; j++)
                Worker::connect(*workers_[i], *workers_[j]);
        }

        // listen
        if (listen(QHostAddress::Any, port))
            qDebug() << "listening on port" << this->serverPort();
        else
            throw std::runtime_error("listen() failed");
    }

    QSslKey Server::private_key() const {
        return private_key_;
    }

    QSslCertificate Server::local_certificate() const {
        return local_certificate_;
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
