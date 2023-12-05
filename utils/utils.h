//
// Created by aichao on 2022/5/11.
//

#pragma once

#include <QImage>
#include <QDebug>
#include <QWidget>
#include "struct.h"
#include "snowflake.hpp"
#include <seeta/Common/Struct.h>
#include<seeta/FaceDetector.h>
#include <opencv2/opencv.hpp>

using snowflake_t = snowflake<1534832906275L, std::mutex>;

class utils {

public:

    static SeetaImageData cvMatToSImg(cv::Mat &img);

    static cv::Mat cropImg(const cv::Mat &img, cv::Size target_size = cv::Size(600, 1024));

    static QImage cvMat2QImage(const cv::Mat &img);

    static cv::Mat qImageToCvMat(const QImage &img);

    static SeetaRect qRectToSRect(const QRect &rect);

    static QRect sRectToQRect(const SeetaRect &s_rect);

    static qint64 getUuid();

    static QByteArray floatArray2QByteArray(float *buffer, int size);

    static void setBackgroundColor(QWidget *w, QColor color);


    template<typename T>
    static QByteArray Vector2QByteArray(const std::vector<T> &vec) {
        QByteArray array;
        int byte_num = sizeof(T) * vec.size();
        array.resize(byte_num);
        memcpy(array.data(), vec.data(), byte_num);
        return array;
    }

    template<typename T>
    static std::vector<T> QByteArray2Vector(const QByteArray &array) {
        std::vector<T> vec;
        int vec_size = array.size() / sizeof(T);
        vec.resize(vec_size);
        memcpy(vec.data(), array.data(), array.size());
        return vec;
    }

    template<typename T>
    static QByteArray CvMat2JPGQByteArray(cv::Mat &img) {
        std::vector<T> buffer;
        cv::imencode(".jpg", img, buffer);
        return vector2ByteArray(buffer);
    }

    template<typename T>
    static cv::Mat JPGQBytearray2CVMat(const QByteArray &array) {
        std::vector<T> buffer = QByteArray2Vector<T>(array);
        return cv::imdecode(buffer, cv::IMREAD_COLOR);
    }

private:

    inline static snowflake_t uuid_ = snowflake_t();
};

