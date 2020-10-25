#include <cte/client_new/client.h>
#include <QtCore/QPointer>
#include <QtCore/QThread>

namespace cte {
    Client::Client(const QString& hostname, int port) {
        // launch thread for network worker
        thread_ = new QThread(this);
        thread_->start();
        network_worker_ = new NetworkWorker();
        network_worker_->moveToThread(thread_);

        // use this thread for UI worker
        ui_worker_ = QSharedPointer<UiWorker>::create();

        // connect workers
        NetworkWorker *network_worker = network_worker_.data();
        UiWorker *ui_worker = ui_worker_.data();
        connect(ui_worker, &UiWorker::connection_request, network_worker, &NetworkWorker::set_server);
        connect(ui_worker, &UiWorker::new_message, network_worker, &NetworkWorker::send_message);
        connect(network_worker, &NetworkWorker::connected, ui_worker, &UiWorker::show_login_form);
        connect(network_worker, &NetworkWorker::new_message, ui_worker, &UiWorker::process_message);
        connect(network_worker, qOverload<const QString&>(&NetworkWorker::error_occurred),
                ui_worker, &UiWorker::show_connection_form);
        connect(network_worker, qOverload<const QString&>(&NetworkWorker::error_occurred),
                ui_worker, qOverload<const QString&>(&UiWorker::show_error));

        // set server
        network_worker_->set_server(hostname, port);
    }

    Client::~Client() {
        thread_->exit();
    }
}
