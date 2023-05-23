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
#include <QMutex>
#include <QJsonDocument>
#include <opencv2/opencv.hpp>
#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceRecognizer.h>
#include <seeta/FaceTracker.h>
#include <seeta/FaceAntiSpoofing.h>
#include "Utils.h"
#include "struct.h"

using Status = seeta::FaceAntiSpoofing::Status;

class SeetaFace {

public:

    static SeetaFace* getInstance(){
        mutex_.lock();
        if(seetaFacePtr == nullptr){
            seetaFacePtr = new SeetaFace();
        }
        mutex_.unlock();
        return seetaFacePtr;
    }

    void create_face_libs(const QString &json_path);

    bool create_face_lib(const QString &img_path, const QString &name, const QString &staff_id);

    bool extract_feature(cv::Mat &img, float *feature);

    SeetaFaceInfoArray face_detection(cv::Mat &img);

    FaceRecRet face_recognition(cv::Mat &img, std::vector<SeetaPointF> points);

    std::vector<SeetaPointF> face_marker(cv::Mat &img, SeetaRect &rect);

    bool face_quality_authorize(cv::Mat &img);

    Status face_anti_spoofing(cv::Mat &img, SeetaRect &rect, std::vector<SeetaPointF> points);


private:
    SeetaFace();
    float m_threshold = 0.6;
    std::string model_dir= "/home/orangepi/my/seeta-face6/build/models/";
    std::vector<FaceLibInfo> face_lib;
    std::shared_ptr<seeta::FaceDetector> FD = nullptr;
    std::shared_ptr<seeta::FaceLandmarker> FL = nullptr;
    std::shared_ptr<seeta::FaceRecognizer> FR = nullptr;
    std::shared_ptr<seeta::FaceAntiSpoofing> FS = nullptr;
    inline static QMutex mutex_;
    inline static SeetaFace* seetaFacePtr = nullptr;
};


#endif //FRAM_SEETAFACE_H
