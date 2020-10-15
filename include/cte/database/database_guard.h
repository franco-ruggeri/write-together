/*
 * Wrapper of QSqlDatabase using the RAII paradigm.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtSql/QSqlDatabase>

namespace cte {
    class DatabaseGuard {
        QSqlDatabase database_;

    public:
        DatabaseGuard(const DatabaseGuard& other) = delete;
        DatabaseGuard& operator=(const DatabaseGuard& other) = delete;

        explicit DatabaseGuard(const QSqlDatabase& database);
        ~DatabaseGuard();
    };
}
