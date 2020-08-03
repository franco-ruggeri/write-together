/*
 * Wrapper of QSqlDatabase using the RAII paradigm.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtSql/QSqlDatabase>

namespace editor {
    class DatabaseGuard {
        QSqlDatabase database_;

        DatabaseGuard(const DatabaseGuard& other) = delete;
        DatabaseGuard& operator=(const DatabaseGuard& other) = delete;

    public:
        DatabaseGuard(const QSqlDatabase& database);
        ~DatabaseGuard();
    };
}
