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
#include "utils/utils.h"
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

    bool addFace(cv::Mat &img, const QString &uid, const QString &name);

    bool deleteFace(const QString &uid);

    void buildFaceIndexFromDb();

    QString getFacePathFromIndexId(qint64 index_id);

    Staff getFaceInfoFromIndexId(qint64 index_id);

    bool updateFace(cv::Mat &img, const QString &uid, const QString &name);

    bool extractFeature(cv::Mat &img, float *feature);

    std::vector<SeetaFaceInfo> faceDetection(cv::Mat &img);

    QPair<int64_t, float> faceRecognition(cv::Mat &img, std::vector<SeetaPointF> points);

    std::vector<SeetaPointF> faceMarker(cv::Mat &img, const SeetaRect &rect);

    bool faceQualityAuthorize(cv::Mat &img);

    Status faceAntiSpoofing(cv::Mat &img, const SeetaRect &rect, std::vector<SeetaPointF> points);

    bool deleteFaceByIds(const std::vector<int64_t> &ids);

    template<class T>
    QVector<T> getQueryInfo(int row_num_pre_page, int current_page_index) {
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
    int getQueryNum() {
        qx_query query;
        int ret = qx::dao::count<T>(query);
        return ret;
    }

private:

    SeetaFace();

    ~SeetaFace() = default;

    SeetaFace(const SeetaFace &);

    SeetaFace &operator=(const SeetaFace &);

    void initFileDir();

    void initFaceDb();

private:
    std::shared_ptr<seeta::FaceDetector> FD_ = nullptr;
    std::shared_ptr<seeta::FaceLandmarker> FL_ = nullptr;
    std::shared_ptr<seeta::FaceRecognizer> FR_ = nullptr;
    std::shared_ptr<seeta::FaceAntiSpoofing> FS_ = nullptr;
};


