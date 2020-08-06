/*
 * Utilities for QtSql with secrete information (credentials, DB structure, etc.), not to be distributed as library.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtGui/QImage>
#include <cte/protocol/Profile.h>
#include <cte/protocol/Document.h>

QSqlDatabase connect_to_database();

// identity management queries
QSqlQuery query_select_profile(const QSqlDatabase& database, const QString& username);
QSqlQuery query_select_profile_for_update(const QSqlDatabase& database, const QString& username);
QSqlQuery query_insert_profile(const QSqlDatabase& database, const cte::Profile& profile, const QString& password);
QSqlQuery query_update_profile(const QSqlDatabase& database, const QString& old_username,
                               const cte::Profile& new_profile, const QString& new_password=QString{});

// document management queries
QSqlQuery query_select_document_for_update(const QSqlDatabase& database, const cte::Document& document);
QSqlQuery query_insert_document(const QSqlDatabase& database, const cte::Document& document,
                                const QString& sharing_link);
QSqlQuery query_insert_sharing(const QSqlDatabase& database, const cte::Document& document, const QString& username);
QSqlQuery query_select_shared_document(const QSqlDatabase& database, const cte::Document& document, const QString& username);
QSqlQuery query_select_document_profiles(const QSqlDatabase& database, const cte::Document& document);
QSqlQuery query_select_document_text(const QSqlDatabase& database, const cte::Document& document);
