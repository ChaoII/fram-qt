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
std::vector<float> L2Normalize(const std::vector<float>& values);
QByteArray mat2ByteArray(const cv::Mat &image);
cv::Mat byteArray2Mat(const QByteArray & byteArray);
QByteArray vectorf2ByteArray(const QVector<float>& vec);
QVector<float> byteArray2Vectorf(const QByteArray& bytes);
} // namespace Utility

#endif // UTILITY_H
