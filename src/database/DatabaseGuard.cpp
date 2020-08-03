/*
 * Author: Franco Ruggeri
 */

#include <editor/database/DatabaseGuard.h>

namespace editor {
    DatabaseGuard::DatabaseGuard(const QSqlDatabase& database) : database_(database) {}

    DatabaseGuard::~DatabaseGuard() {
        if (database_.rollback());  // nothing happens if a transaction has not been started
        database_.close();
    }
}
