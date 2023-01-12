#include "plugin.h"
#include "core/attend.h"
#include "core/facehelper.h"
#include "core/staff.h"
#include "core/vectorsearch.h"

void Plugin::register_orm() {
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("./data.db");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);

    QSqlError staff_error = qx::dao::create_table<Staff>();
    if (staff_error.isValid()) {
        qDebug() << staff_error.text();
    }
    QSqlError attend_error = qx::dao::create_table<Attend>();
    if (attend_error.isValid()) {
        qDebug() << attend_error.text();
    }
}

void Plugin::register_facehelper() { FaceHelper::getInstance(); }

void Plugin::register_vectorhelper() { VectorSearch::getInstance(); }
