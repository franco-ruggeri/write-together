#pragma once

#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QThread>
#include <cte/client/worker_network.h>
#include <cte/client/worker_ui.h>

namespace cte {
    class Client : public QObject {
        Q_OBJECT

        QPointer<NetworkWorker> network_worker_;
        QSharedPointer<UiWorker> ui_worker_;
        QPointer<QThread> thread_;

    public:
        Client();
        ~Client();
        void start(const QString& hostname, int port);  // parameters refer to server
    };
}
