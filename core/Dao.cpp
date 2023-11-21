//
// Created by AC on 2023/11/21.
//

#include "Dao.h"


bool Dao::deleteAttendHistoryByUpBound(const QString &endTime) {
    QVector<Attend> attends;
    qx_query query;
    query.where("attend_time").isLessThan(endTime);
    qx::dao::fetch_by_query(query, attends);
    for (auto &attend: attends) {
        QFileInfo file_info(attend.pic_url);
        if (file_info.exists()) {
            file_info.dir().remove(file_info.fileName());
            if (file_info.dir().entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).count() <= 0) {
                file_info.dir().removeRecursively();
                qDebug() << "清空文件夹:" << file_info.dir().absolutePath();
            }
            qDebug() << attend.uid << attend.name << file_info.fileName();
        }
    }
    QSqlError sql_error = qx::dao::delete_by_query<Attend>(query);
    if (sql_error.isValid()) {
        qDebug() << "delete data error , details:" << sql_error.text();
        return false;
    }
    return true;
}
