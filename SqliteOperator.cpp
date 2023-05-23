//
// Created by aichao on 2022/5/12.
//

#include "SqliteOperator.h"

SqliteOperator::SqliteOperator(const QString &db_name) {
    _db_name = db_name;
}


void SqliteOperator::create_db(const QString &conn_name) {

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        _db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        _db = QSqlDatabase::addDatabase("QSQLITE");
        _db.setDatabaseName(_db_name);
    }
}

bool SqliteOperator::open_db() {
    if (!_db.open()) {
        qDebug() << "Error: Failed to connect database." << _db.lastError();
        return false;
    }
    return true;
}


void SqliteOperator::close_db() {
    _db.close();
}

void SqliteOperator::create_table(const QString &creat_sql) {
    QSqlQuery sql_query;
    sql_query.prepare(creat_sql);
    if (!sql_query.exec()) {
        qWarning() << "Error: Fail to create table." << sql_query.lastError();
    } else {
        qInfo() << "Table created!";
    }
}

void SqliteOperator::insert_data(const QString &insert_sql) {
    QSqlQuery sql_query;
    sql_query.prepare(insert_sql);
    if (!sql_query.exec()) {
        qWarning() << sql_query.lastError();
    } else {
        qInfo() << "record insert successfully!";
    }
}

void SqliteOperator::update_data(const QString &update_sql) {

    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        qDebug() << "updated!";
    }

}

QSqlQuery SqliteOperator::query_all() {
    QString select_all_sql = "select * from student";
    QSqlQuery sql_query;
    sql_query.prepare(select_all_sql);
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    }
    return sql_query;
}

QSqlQuery SqliteOperator::query_data(const QString &select_sql) {
    QSqlQuery sql_query;
    if (!sql_query.exec(select_sql)) {
        qDebug() << sql_query.lastError();
    }

    return sql_query;

}



