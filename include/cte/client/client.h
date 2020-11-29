/*
 * Client controller. It launches and connects network and UI workers.
 */

#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QThread>
#include <cte/client/worker_network.h>
#include <cte/client/worker_ui.h>

namespace cte {
    class Client : public QObject {
        Q_OBJECT

        QSharedPointer<UiWorker> ui_worker_;
        QSharedPointer<NetworkWorker> network_worker_;

    public:
        Client();
        void start(const QString& hostname, int port);      // parameters refer to server
    };
}
