/*
 * Author: Franco Ruggeri
 */

#include "db_utility.h"
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtSql/QSqlError>

static const QString driver_type("QMYSQL");
static const QString database_name("collaborative_text_editor");
static const QString hostname("127.0.0.1");
static const QString username("collaborative_text_editor");
static const QString password("?PdSPr0j3ct!");

QSqlDatabase connect_to_database() {
    QString name = reinterpret_cast<char *>(QThread::currentThread());
    QSqlDatabase database = QSqlDatabase::database(name);

    // first time
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase(driver_type, name);
        database.setDatabaseName(database_name);
        database.setHostName(hostname);
        database.setUserName(username);
        database.setPassword(password);
        database.open();
    }

    if (!database.isOpen()) {
        qDebug() << database.lastError();
        throw std::runtime_error("connection to database failed");
    }

    return database;
}
