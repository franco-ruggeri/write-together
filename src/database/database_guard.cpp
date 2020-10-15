/*
 * Author: Franco Ruggeri
 */

#include <cte/database/database_guard.h>

namespace cte {
    DatabaseGuard::DatabaseGuard(const QSqlDatabase& database) : database_(database) {}

    DatabaseGuard::~DatabaseGuard() {
        database_.rollback();   // nothing happens if a transaction has not been started
        database_.close();
    }
}
