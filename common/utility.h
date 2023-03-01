#ifndef UTILITY_H
#define UTILITY_H

#include "common/snowflake.hpp"
#include <QImage>
#include <QtWidgets>
#include <iostream>
#include <opencv2/opencv.hpp>

using snowflake_t = snowflake<1534832906275L, std::mutex>;

namespace Utility {

static snowflake_t uuid;
QImage Mat2QImage(cv::Mat &mat);
cv::Mat QImage2Mat(QImage image);
cv::Mat CropImage(cv::Mat &mat, std::vector<int> box);
cv::Mat fit_screen(const cv::Mat &mat, QSize screen_size);
QImage fit_screen(const QImage &img, QSize screen_size);
qint64 get_uuid();
QStringList get_file_lists(const QString &dirPath);
std::vector<float> L2Normalize(const std::vector<float> &values);
QByteArray mat2ByteArray(const cv::Mat &image);
cv::Mat byteArray2Mat(const QByteArray &byteArray);
template <class T> QByteArray vector2ByteArray(const QVector<T> &vec) {

    QByteArray array;
    // 计算vec的字节数
    int vec_byte_count = sizeof(T) * vec.size();
    // 分配内存
    array.resize(vec_byte_count);
    // 内存拷贝
    memcpy(array.data(), vec.data(), vec_byte_count);
    return array;
}
template <class T> QVector<T> byteArray2Vector(const QByteArray &bytes) {
    QVector<T> vec;
    int vec_size = bytes.size() / sizeof(T);
    vec.resize(vec_size);
    // 拷贝的字节数
    memcpy(vec.data(), bytes.data(), bytes.size());
    return vec;
}
template <class T> QByteArray matJPG2ByteArray(const cv::Mat &image) {
    std::vector<T> buffer;
    cv::imencode(".jpg", image, buffer);

    return vector2ByteArray<T>(QVector<T>(buffer.begin(),buffer.end()));
}

template <class T> cv::Mat byteArrayJPG2Mat(const QByteArray &byteArray) {
    std::vector<T> buffer = byteArray2Vector<T>(byteArray).toVector();
    return cv::imdecode(buffer, cv::IMREAD_COLOR);
}
} // namespace Utility

#endif // UTILITY_H
