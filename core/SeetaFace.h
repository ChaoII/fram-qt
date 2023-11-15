//
// Created by aichao on 2022/5/11.
//

#pragma once


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
#include "utils/Utils.h"
#include "utils/struct.h"
#include "models/models.h"
#include "core/vectorsearch.h"


using Status = seeta::FaceAntiSpoofing::Status;

class SeetaFace {

public:

    static SeetaFace &getInstance() {
        static SeetaFace seetaFace;
        return seetaFace;
    }

    bool add_face(cv::Mat &img, const QString &uid, const QString &name);

    bool delete_face(const QString &uid);

    void build_face_index_from_db();

    QString get_facepath_from_index_id(qint64 index_id);

    Staff get_faceinfo_from_index_id(qint64 index_id);

    bool update_face(cv::Mat &img, const QString &uid, const QString &name);

    bool extract_feature(cv::Mat &img, float *feature);

    std::vector<SeetaFaceInfo> face_detection(cv::Mat &img);

    QPair<int64_t, float> face_recognition(cv::Mat &img, std::vector<SeetaPointF> points);

    std::vector<SeetaPointF> face_marker(cv::Mat &img, const SeetaRect &rect);

    bool face_quality_authorize(cv::Mat &img);

    Status face_anti_spoofing(cv::Mat &img, const SeetaRect &rect, std::vector<SeetaPointF> points);

    bool delete_face_by_ids(const std::vector<int64_t> &ids);

    template<class T>
    QVector<T> get_query_info(int row_num_pre_page, int current_page_index) {
        QVector<T> models;
        qx_query query;
        int start_row = current_page_index * row_num_pre_page;
        if (std::is_same<T, Attend>()) {
            query.orderDesc("attend_time").limit(row_num_pre_page, start_row);
        } else {
            query.orderDesc("register_time").limit(row_num_pre_page, start_row);
        }

        QSqlError sql_error = qx::dao::fetch_by_query(query, models);
        if (sql_error.isValid()) {
            qDebug() << "select data error, details:" << sql_error.text();
        }
        return models;
    }

    template<class T>
    int get_query_num() {
        qx_query query;
        int ret = qx::dao::count<T>(query);
        return ret;
    }

private:

    SeetaFace();

    ~SeetaFace() = default;

    SeetaFace(const SeetaFace &);

    SeetaFace &operator=(const SeetaFace &);

    void init_file_dir();

    void init_face_db();

private:
    std::shared_ptr<seeta::FaceDetector> FD = nullptr;
    std::shared_ptr<seeta::FaceLandmarker> FL = nullptr;
    std::shared_ptr<seeta::FaceRecognizer> FR = nullptr;
    std::shared_ptr<seeta::FaceAntiSpoofing> FS = nullptr;
};


