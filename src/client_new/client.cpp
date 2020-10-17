#include <cte/client_new/client.h>
#include <QtCore/QPointer>
#include <QtCore/QThread>

namespace cte {
    Client::Client(const QString& hostname, int port) {
        // launch thread for network worker
        thread_ = new QThread(this);
        thread_->start();
        network_worker_ = QSharedPointer<NetworkWorker>::create();
        network_worker_->moveToThread(thread_);

        // use this thread for UI worker
        ui_worker_ = QSharedPointer<UiWorker>::create();

        // connect workers
        NetworkWorker *nw = network_worker_.data();
        UiWorker *uw = ui_worker_.data();
        connect(nw, &NetworkWorker::connected, uw, &UiWorker::show_login_form);
        connect(nw, &NetworkWorker::new_message, uw, &UiWorker::process_message);
        connect(uw, &UiWorker::new_message, nw, &NetworkWorker::send_message);

        // set server
        network_worker_->set_server(hostname, port);
    }

    Client::~Client() {
        thread_->exit();
    }
}
