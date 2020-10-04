/*
 * Authors: Antonino Musmeci, Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace cte {
    bool check_ip_address(const QString& address);
}
