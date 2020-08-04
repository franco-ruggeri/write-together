/*
 * Author: Franco Ruggeri
 */

#include "db_utility.h"
#include <editor/database/db_utility.h>

static const QString driver_type("QMYSQL");
static const QString database_name("collaborative_text_editor");
static const QString hostname("127.0.0.1");
static const QString username("collaborative_text_editor");
static const QString password("?PdSPr0j3ct!");

QSqlDatabase connect_to_database() {
    return editor::connect_to_database(driver_type, database_name, hostname, username, password);
}