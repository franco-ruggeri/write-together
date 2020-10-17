#pragma once

#include <cte/client_new/network_worker.h>
#include <cte/client_new/ui_worker.h>

namespace cte {
    class Client : public QObject {
        Q_OBJECT

        QSharedPointer<NetworkWorker> network_worker_;
        QSharedPointer<UiWorker> ui_worker_;
        QPointer<QThread> thread_;

    public:
        Client(const QString& hostname, int port);
        ~Client();
    };
}
