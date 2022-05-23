//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_UTILS_H
#define FRAM_UTILS_H

#include <QImage>
#include <QDebug>
#include "include/struct.h"
#include <seeta/Struct.h>
#include <opencv2/opencv.hpp>

class Utils {

public:
    static SeetaImageData cvMat_2_img(cv::Mat &img);

    static cv::Mat crop_img(const cv::Mat &img, cv::Size target_size = cv::Size(600, 1024));

    static QImage cvMat_2_qimg(const cv::Mat &img);

};


#endif //FRAM_UTILS_H
