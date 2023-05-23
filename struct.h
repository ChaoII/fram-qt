//
// Created by aichao on 2022/5/12.
//

#ifndef FRAM_STRUCT_H
#define FRAM_STRUCT_H

#include "Utils.h"

// 人脸库
struct FaceLibInfo {
    QString name;
    QString staff_id;
    std::shared_ptr<float> feature;
};

// 人脸识别结果
struct FaceRecRet {
    QString face_id;
    QString name;
    float score;

};

// 人脸识别结果封装
struct FaceInfoWrap {
    int code;
    QString time;
    FaceRecRet ret;
};


#endif //FRAM_STRUCT_H
