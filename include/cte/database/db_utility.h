/*
 * Utilities for QtSql.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace cte {
    QSqlDatabase connect_to_database(const QString& driver_type, const QString& database_name, const QString& hostname,
                                     const QString& username, const QString& password);

    void execute_query(QSqlQuery& query);
}
