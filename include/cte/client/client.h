#pragma once

#include <cte/client/worker_network.h>
#include <cte/client/worker_ui.h>

namespace cte {
    class Client : public QObject {
        Q_OBJECT

        QPointer<NetworkWorker> network_worker_;
        QSharedPointer<UiWorker> ui_worker_;
        QPointer<QThread> thread_;

    public:
        Client(const QString& hostname, int port);
        ~Client();
    };
}
