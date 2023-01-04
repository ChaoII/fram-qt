#ifndef FACEHELPER_H
#define FACEHELPER_H

#include "common/config.h"

#include <QtWidgets>
#include <fastdeploy/vision.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class FaceHelper {

public:
    /// initial_facelibs
    void initial_facelibs();
    QMap<qint64, FaceInfo> get_facelibs();
    void add_database(cv::Mat img, QString uid, QString name);
    void delete_face(QString uid);
    cv::Rect get_face_box(cv::Mat img);
    cv::Mat get_face_roi(cv::Mat img);
    std::vector<float> get_face_feature(cv::Mat img);
    static FaceHelper *getInstance();
    void delete_instance();

private:
    explicit FaceHelper();
    ~FaceHelper() = default;
    fastdeploy::vision::facedet::SCRFD *face_det_;
    fastdeploy::vision::faceid::ArcFace *face_rec_;
    static QMutex mutex_;
    static FaceHelper *face_helpher_instance_;
    QMap<qint64, FaceInfo> face_libs_;
};

#endif // FACEHELPER_H
