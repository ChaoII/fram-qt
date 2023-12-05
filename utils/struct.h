//
// Created by aichao on 2022/5/12.
//

#pragma once

#include <QDataStream>
#include "utils/utils.h"
#include <QMetaObject>
#include <opencv2/opencv.hpp>

struct FaceInfo {
    QString uid;
    QString name;
    QVector<float> feature;

    friend QDataStream &operator<<(QDataStream &output, const FaceInfo &face_info) {
        output << face_info.uid << face_info.name << face_info.feature;
        return output;
    }

    friend QDataStream &operator>>(QDataStream &input, FaceInfo &face_info) {
        input >> face_info.uid >> face_info.name >> face_info.feature;
        return input;
    }
};

// 人脸库
struct FaceLibInfo {
    QString name;
    QString staff_id;
    std::shared_ptr<float> feature;
};

// 人脸识别结果
struct FaceRecRet {
    QString uid;
    QString name;
    QString pic_url;
    QImage img;
    float score;
};

enum class RecognitionStatus {
    Success,
    Unknown,
    SPOOF,
    None
};

// 人脸识别结果封装
struct FaceInfoWrap {
    RecognitionStatus status = RecognitionStatus::Unknown;
    QString time;
    FaceRecRet ret;
};

struct SearchResult {
    std::vector<int64_t> I;
    std::vector<float> D;
    int return_k;
};

Q_DECLARE_METATYPE(FaceInfoWrap)

Q_DECLARE_METATYPE(QVector<FaceInfoWrap>);

