/*
 * Author: Franco Ruggeri
 */

#include <editor/database/db_utility.h>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtSql/QSqlError>

namespace editor {
    QSqlDatabase connect_to_database(const QString& driver_type, const QString& database_name, const QString& hostname,
                                     const QString& username, const QString& password) {
        // different connections for different threads, to exploit multiple cores
        QString connection_name;
        QDebug stream(&connection_name);
        stream << QThread::currentThreadId();
        QSqlDatabase database = QSqlDatabase::database(connection_name);

        // first time
        if (!database.isValid()) {
            database = QSqlDatabase::addDatabase(driver_type, connection_name);
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

    void execute_query(QSqlQuery& query) {
        if (!query.exec()) {
            qDebug() << query.lastError();
            throw std::runtime_error("query to database failed");
        }
    }
}
