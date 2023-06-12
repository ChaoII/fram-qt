//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACE_H
#define FRAM_SEETAFACE_H


#define TOSTR(ARG) #ARG

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
#include "models.h"
#include "vectorsearch.h"


using Status = seeta::FaceAntiSpoofing::Status;

class SeetaFace {

public:

    static SeetaFace* getInstance(){
        if(seetaFacePtr == nullptr){
            std::lock_guard<std::mutex> lock(mutex_);
            if(seetaFacePtr == nullptr){
                seetaFacePtr = new SeetaFace();
            }
        }
        return seetaFacePtr;
    }



    bool add_face(cv::Mat &img, const QString &uid, const QString &name);

    bool delete_face(const QString &uid);

    void build_face_index_from_db();

    QString get_facepath_from_index_id(qint64 index_id);

    Staff get_faceinfo_from_index_id(qint64 index_id);

    bool update_face(cv::Mat &img, const QString &uid, const QString &name);

    bool extract_feature(cv::Mat &img, float *feature);

    std::vector<SeetaFaceInfo> face_detection(cv::Mat &img);

    QPair<int64_t,float> face_recognition(cv::Mat &img, std::vector<SeetaPointF> points);

    std::vector<SeetaPointF> face_marker(cv::Mat &img, const SeetaRect &rect);

    bool face_quality_authorize(cv::Mat &img);

    Status face_anti_spoofing(cv::Mat &img, const SeetaRect &rect, std::vector<SeetaPointF> points);

    QVector<Attend> get_attend_info(int row_num_pre_page, int current_page_index);
    int get_attend_num();

private:
    SeetaFace();

    SeetaFace(const SeetaFace&);

    SeetaFace& operator=(const SeetaFace&);

    void init_file_dir();

    void init_face_db();


private:
    int global_thread_nums = 1;
    std::string model_dir= "/home/orangepi/my/seeta-face6/build/models/";
    std::vector<FaceLibInfo> face_lib;
    std::shared_ptr<seeta::FaceDetector> FD = nullptr;
    std::shared_ptr<seeta::FaceLandmarker> FL = nullptr;
    std::shared_ptr<seeta::FaceRecognizer> FR = nullptr;
    std::shared_ptr<seeta::FaceAntiSpoofing> FS = nullptr;
    inline static std::mutex mutex_;
    inline static SeetaFace* seetaFacePtr = nullptr;
};


#endif //FRAM_SEETAFACE_H
