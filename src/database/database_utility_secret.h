/*
 * Utilities for QtSql with secrete information (credentials, DB structure, etc.), not to be distributed as library.
 */

#pragma once

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtGui/QImage>
#include <cte/protocol/profile.h>
#include <cte/protocol/document.h>

namespace cte {
    QSqlDatabase connect_to_database();

    // identity management queries
    QSqlQuery query_select_profile(const QSqlDatabase& database, const QString& username, bool for_update=false);
    QSqlQuery query_insert_profile(const QSqlDatabase& database, const Profile& profile, const QString& password);
    QSqlQuery query_update_profile(const QSqlDatabase& database, const Profile& new_profile,
                                   const QString& new_password=QString{});

    // document management queries
    QSqlQuery query_select_document(const QSqlDatabase& database, const Document& document, bool for_update=false);
    QSqlQuery query_select_document(const QSqlDatabase& database, const Document& document, const QString& username,
                                    bool for_update=false);
    QSqlQuery query_select_document(const QSqlDatabase& database, const QUrl& sharing_link);
    QSqlQuery query_select_document_profiles(const QSqlDatabase& database, const Document& document);
    QSqlQuery query_select_document_text(const QSqlDatabase& database, const Document& document);
    QSqlQuery query_select_documents(const QSqlDatabase& database, const QString& username);
    QSqlQuery query_insert_document(const QSqlDatabase& database, const Document& document, const QUrl& sharing_link);
    QSqlQuery query_insert_sharing(const QSqlDatabase& database, const Document& document, const QString& username);

    // document editing queries
    QSqlQuery query_delete_document_text(const QSqlDatabase& database, const Document& document);
    QSqlQuery prepare_query_insert_character(const QSqlDatabase& database, const Document& document);
    void bind_query_insert_character(QSqlQuery& query, unsigned int index, const QString& author, QChar value);

}
