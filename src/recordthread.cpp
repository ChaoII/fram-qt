//
// Created by aichao on 2022/5/12.
//


#include "include/recordthread.h"


RecordThread::RecordThread() {

//    _sql_helper = std::make_shared<SqliteOperator>("attend.db");
//    _sql_helper->create_db();

}


void RecordThread::update_info(QVector<FaceInfoWrap> &face_info) {

    _face_infos = face_info;

}

void RecordThread::run() {

    SqliteOperator sql_helper("attend.db");
    sql_helper.create_db();

    for (auto &face_info: _face_infos) {
        auto ret = face_info.ret;
        QString name = ret.name;
        QString staff_id = ret.face_id;
        QString attend_time = face_info.time;
        sql_helper.open_db();
        QString insert_sql = QString("insert into attend (staff_id,name,attend_time) values ('%1','%2','%3')").arg(
                staff_id).arg(
                name).arg(attend_time);
        qDebug() << insert_sql;
        sql_helper.insert_data(insert_sql);
    }
    sql_helper.close_db();
}


RecordThread::~RecordThread() {


}
