//
// Created by aichao on 2022/5/12.
//


#include "recordthread.h"
#include "models/models.h"

void RecordThread::record(QVector<FaceInfoWrap> face_infos) {

    unique_record(face_infos);
    QString date_str = QDateTime::currentDateTime().toString("yyyyMMdd");
    QDir dir(QDir::currentPath()+"/attend/"+date_str);
    if(!dir.exists()){
        dir.mkdir(dir.absolutePath());
    }
    QVector<Attend> attends;
    for (auto &face_info: face_infos) {
        Attend attend;
        QString file_path = dir.absoluteFilePath(QDateTime::currentDateTime()
                                                 .toString("yyyyMMddhhmmsszzzzzz")+"_"
                                                 +QString(face_info.ret.uid)+".jpg");
        attend.name = face_info.ret.name;
        attend.uid = face_info.ret.uid;
        attend.pic_url = file_path;
        attend.attend_time = face_info.time;
        face_info.ret.img.save(file_path);
        attends.append(attend);
    }
    auto sql_error = qx::dao::insert(attends);
    if(sql_error.isValid()){
       qDebug()<<sql_error.text();
    }
}

void RecordThread::unique_record(QVector<FaceInfoWrap> &vec) {
    std::sort(vec.begin(), vec.end(), [](FaceInfoWrap &vec1, FaceInfoWrap &vec2) {
        return vec1.ret.uid < vec2.ret.uid;
    });
    for (int i = 0; i < vec.size(); i++) {
        for (int j = i + 1; j < vec.size(); j++) {
            if (vec[i].ret.uid == vec[j].ret.uid) {
                vec.remove(j);
                j--;
            }
        }
    }
}

RecordThread::~RecordThread() {


}
