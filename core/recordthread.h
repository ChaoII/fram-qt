//
// Created by aichao on 2022/5/12.
//

#pragma once

#include "utils/struct.h"
#include "utils/Utils.h"
#include <QObject>


class RecordThread : public QObject {
Q_OBJECT

public:
    explicit RecordThread(QObject *parent = nullptr) : QObject(parent) {};

    ~RecordThread();

public slots:

    void record(QVector<FaceInfoWrap>);

private:
    void unique_record(QVector<FaceInfoWrap> &vec);

};

