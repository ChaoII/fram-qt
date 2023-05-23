//
// Created by aichao on 2022/5/12.
//

#ifndef FRAM_RECORDTHREAD_H
#define FRAM_RECORDTHREAD_H

#include "struct.h"
#include "Utils.h"
#include "SqliteOperator.h"
#include <QThread>


class RecordThread : public QThread {
Q_OBJECT

public:
    explicit RecordThread(QObject *parent = nullptr);

    void update_info(QVector<FaceInfoWrap> &face_info);

    void run() override;

    void unique_record(QVector<FaceInfoWrap> &vec);

    ~RecordThread();

private:
    QSqlDatabase _db;
    QVector<FaceInfoWrap> _face_infos;
};


#endif //FRAM_RECORDTHREAD_H
