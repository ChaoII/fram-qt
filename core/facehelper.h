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
    void refresh_faceindex();
    QMap<qint64,FaceInfo> get_facelibs();
    FaceInfo get_face_info_from_id(qint64 id);
    void add_database(cv::Mat img, const QString& uid, const QString& name);
    void delete_face(const QString& uid);
    cv::Rect get_face_box(cv::Mat img);
    cv::Mat get_face_roi(cv::Mat img);
    std::vector<float> get_face_feature(const cv::Mat &img);
    static FaceHelper *getInstance();
    void delete_instance();


private:
    explicit FaceHelper();
    ~FaceHelper() = default;
    fastdeploy::vision::facedet::SCRFD *face_det_;
    fastdeploy::vision::faceid::ArcFace *face_rec_;
    static QMutex mutex_;
    static FaceHelper *face_helpher_instance_;
    QMap<qint64,FaceInfo> m_facelibs;
};

#endif // FACEHELPER_H
