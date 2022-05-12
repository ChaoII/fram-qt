//
// Created by aichao on 2022/5/12.
//

#ifndef FRAM_STRUCT_H
#define FRAM_STRUCT_H

#include "include/Utils.h"

struct FaceLibInfo {
    QString name;
    QString staff_id;
    std::shared_ptr<float> feature;
};

struct FaceRecRet {
    QString face_id;
    QString name;
    float score;

};

struct FaceInfoWrap {
    int code;
    QString time;
    FaceRecRet ret;
};


#endif //FRAM_STRUCT_H
