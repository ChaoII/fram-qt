//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACE_H
#define FRAM_SEETAFACE_H


#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QFile>
#include <QVector>
#include <QDebug>
#include <QJsonDocument>
#include <opencv2/opencv.hpp>
#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceRecognizer.h>
#include <seeta/FaceTracker.h>
#include <seeta/FaceAntiSpoofing.h>
#include "include/Utils.h"

using Status = seeta::FaceAntiSpoofing::Status;

class SeetaFace {

public:

    struct FaceLibInfo {
        QString name;
        QString staff_id;
        float *feature;
    };

    struct FaceRecRes {
        QString face_id;
        QString name;
        float score;

    };

    explicit SeetaFace();

    void create_face_libs(const QString &json_path);

    bool create_face_lib(const QString &img_path, const QString &name, const QString &staff_id);

    bool extract_feature(cv::Mat &img, float *feature);

    FaceRecRes face_recognition(cv::Mat &img, QVector<SeetaPointF> points);

    QVector<SeetaPointF> face_marker(cv::Mat &img, SeetaRect &rect);

    bool face_quality_authorize(cv::Mat &img);

    Status face_anti_spoofing(cv::Mat &img, SeetaRect &rect, QVector<SeetaPointF> points);


private:
    float m_threshold;
    std::string model_dir;
    QVector<FaceLibInfo> face_lib;
    std::shared_ptr<seeta::FaceDetector> FD = nullptr;
    std::shared_ptr<seeta::FaceLandmarker> FL = nullptr;
    std::shared_ptr<seeta::FaceRecognizer> FR = nullptr;
    std::shared_ptr<seeta::FaceTracker> FT = nullptr;
    std::shared_ptr<seeta::FaceAntiSpoofing> FS = nullptr;
};


#endif //FRAM_SEETAFACE_H
