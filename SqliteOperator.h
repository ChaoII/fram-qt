//
// Created by aichao on 2022/5/12.
//

#ifndef FRAM_SQLITEOPERATOR_H
#define FRAM_SQLITEOPERATOR_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SqliteOperator {

public:
    SqliteOperator(const QString &db_name);

    void create_db(const QString &conn_name = "qt_sql_default_connection");

    bool open_db();

    void close_db();

    void create_table(const QString &creat_sql);

    void insert_data(const QString &insert_sql);

    void update_data(const QString &update_sql);

    QSqlQuery query_all();

    QSqlQuery query_data(const QString &select_sql);


private:
    QString _db_name;
    QSqlDatabase _db;

};


#endif //FRAM_SQLITEOPERATOR_H
